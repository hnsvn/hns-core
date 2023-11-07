/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_p3a.h"

#include <string>
#include <utility>

#include "base/metrics/histogram_functions.h"
#include "base/metrics/histogram_macros.h"
#include "base/strings/string_number_conversions.h"
#include "base/time/time.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/p3a_utils/bucket.h"
#include "hns/components/p3a_utils/feature_usage.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/scoped_user_pref_update.h"

namespace hns_wallet {

const char kKeyringCreatedHistogramName[] = "Hns.Wallet.KeyringCreated";
const char kOnboardingConversionHistogramName[] =
    "Hns.Wallet.OnboardingConversion.3";
const char kNewUserBalanceHistogramName[] = "Hns.Wallet.NewUserBalance";
const char kEthProviderHistogramName[] = "Hns.Wallet.EthProvider.4";
const char kSolProviderHistogramName[] = "Hns.Wallet.SolProvider.2";
const char kEthTransactionSentHistogramName[] =
    "Hns.Wallet.EthTransactionSent";
const char kSolTransactionSentHistogramName[] =
    "Hns.Wallet.SolTransactionSent";
const char kFilTransactionSentHistogramName[] =
    "Hns.Wallet.FilTransactionSent";
const char kEthActiveAccountHistogramName[] = "Hns.Wallet.ActiveEthAccounts";
const char kSolActiveAccountHistogramName[] = "Hns.Wallet.ActiveSolAccounts";
const char kFilActiveAccountHistogramName[] = "Hns.Wallet.ActiveFilAccounts";
const char kHnsWalletDailyHistogramName[] = "Hns.Wallet.UsageDaily";
const char kHnsWalletWeeklyHistogramName[] = "Hns.Wallet.UsageWeekly";
const char kHnsWalletMonthlyHistogramName[] = "Hns.Wallet.UsageMonthly";
const char kHnsWalletNewUserReturningHistogramName[] =
    "Hns.Wallet.NewUserReturning";
const char kHnsWalletLastUsageTimeHistogramName[] =
    "Hns.Wallet.LastUsageTime";
const char kHnsWalletNFTCountHistogramName[] = "Hns.Wallet.NFTCount";
const char kHnsWalletNFTNewUserHistogramName[] = "Hns.Wallet.NFTNewUser";
const char kHnsWalletNFTDiscoveryEnabledHistogramName[] =
    "Hns.Wallet.NFTDiscoveryEnabled";

namespace {

const int kRefreshP3AFrequencyHours = 24;
const int kActiveAccountBuckets[] = {0, 1, 2, 3, 7};
const char* kTimePrefsToMigrateToLocalState[] = {kHnsWalletLastUnlockTime,
                                                 kHnsWalletP3AFirstUnlockTime,
                                                 kHnsWalletP3ALastUnlockTime};
const char* kTimePrefsToRemove[] = {kHnsWalletP3AFirstReportTime,
                                    kHnsWalletP3ALastReportTime};
const int kNFTCountBuckets[] = {0, 4, 20};
constexpr base::TimeDelta kOnboardingRecordDelay = base::Seconds(120);

// Has the Wallet keyring been created?
// 0) No, 1) Yes
void RecordKeyringCreated(mojom::KeyringInfoPtr keyring_info) {
  UMA_HISTOGRAM_BOOLEAN(kKeyringCreatedHistogramName,
                        static_cast<int>(keyring_info->is_keyring_created));
}

}  // namespace

HnsWalletP3A::HnsWalletP3A(HnsWalletService* wallet_service,
                               KeyringService* keyring_service,
                               PrefService* profile_prefs,
                               PrefService* local_state)
    : wallet_service_(wallet_service),
      keyring_service_(keyring_service),
      profile_prefs_(profile_prefs),
      local_state_(local_state) {
  DCHECK(profile_prefs);
  DCHECK(local_state);

  MigrateUsageProfilePrefsToLocalState();

  RecordInitialHnsWalletP3AState();
  AddObservers();

  local_state_change_registrar_.Init(local_state_);
  local_state_change_registrar_.Add(
      kHnsWalletLastUnlockTime,
      base::BindRepeating(&HnsWalletP3A::ReportUsage, base::Unretained(this),
                          true));
  profile_pref_change_registrar_.Init(profile_prefs_);
  profile_pref_change_registrar_.Add(
      kHnsWalletNftDiscoveryEnabled,
      base::BindRepeating(&HnsWalletP3A::ReportNftDiscoverySetting,
                          base::Unretained(this)));

  // try to record the onboarding histogram
  // just in the case the user quit the app
  // before the 120 second deadline in the last
  // app session
  RecordOnboardingHistogram();

  ReportNftDiscoverySetting();
}

HnsWalletP3A::HnsWalletP3A() = default;

HnsWalletP3A::~HnsWalletP3A() = default;

void HnsWalletP3A::AddObservers() {
  keyring_service_->AddObserver(
      keyring_service_observer_receiver_.BindNewPipeAndPassRemote());
  update_timer_.Start(FROM_HERE, base::Hours(kRefreshP3AFrequencyHours), this,
                      &HnsWalletP3A::OnUpdateTimerFired);
  OnUpdateTimerFired();  // Also call on startup
}

mojo::PendingRemote<mojom::HnsWalletP3A> HnsWalletP3A::MakeRemote() {
  mojo::PendingRemote<mojom::HnsWalletP3A> remote;
  receivers_.Add(this, remote.InitWithNewPipeAndPassReceiver());
  return remote;
}

void HnsWalletP3A::Bind(
    mojo::PendingReceiver<mojom::HnsWalletP3A> receiver) {
  receivers_.Add(this, std::move(receiver));
}

void HnsWalletP3A::ReportUsage(bool unlocked) {
  VLOG(1) << "Wallet P3A: starting report";
  base::Time wallet_last_used =
      local_state_->GetTime(kHnsWalletLastUnlockTime);

  if (unlocked) {
    p3a_utils::RecordFeatureUsage(local_state_, kHnsWalletP3AFirstUnlockTime,
                                  kHnsWalletP3ALastUnlockTime);
    WriteUsageStatsToHistogram();
  } else {
    // Maybe record existing timestamp in case the user is not new.
    p3a_utils::MaybeRecordFeatureExistingUsageTimestamp(
        local_state_, kHnsWalletP3AFirstUnlockTime,
        kHnsWalletP3ALastUnlockTime, wallet_last_used);
  }

  p3a_utils::RecordFeatureNewUserReturning(
      local_state_, kHnsWalletP3AFirstUnlockTime,
      kHnsWalletP3ALastUnlockTime, kHnsWalletP3AUsedSecondDay,
      kHnsWalletNewUserReturningHistogramName);
  p3a_utils::RecordFeatureLastUsageTimeMetric(
      local_state_, kHnsWalletP3ALastUnlockTime,
      kHnsWalletLastUsageTimeHistogramName);

  ReportNftDiscoverySetting();
}

void HnsWalletP3A::ReportJSProvider(mojom::JSProviderType provider_type,
                                      mojom::CoinType coin_type,
                                      bool allow_provider_overwrite) {
  CHECK(coin_type == mojom::CoinType::ETH || coin_type == mojom::CoinType::SOL);

  const auto keyring_id =
      keyring_service_->GetKeyringIdForCoinNonFIL(coin_type);
  CHECK(keyring_id.has_value());

  const char* histogram_name;
  switch (coin_type) {
    case mojom::CoinType::ETH:
      histogram_name = kEthProviderHistogramName;
      break;
    case mojom::CoinType::SOL:
      histogram_name = kSolProviderHistogramName;
      break;
    default:
      NOTREACHED_NORETURN();
  }

  JSProviderAnswer answer = JSProviderAnswer::kNoWallet;
  bool is_wallet_setup = keyring_service_->IsKeyringCreated(*keyring_id);

  switch (provider_type) {
    case mojom::JSProviderType::None:
      if (is_wallet_setup) {
        answer = JSProviderAnswer::kWalletDisabled;
      } else {
        answer = JSProviderAnswer::kNoWallet;
      }
      break;
    case mojom::JSProviderType::ThirdParty:
      // Third-party overriding is considered if the native wallet
      // is enabled and the native wallet is setup.
      answer = is_wallet_setup && allow_provider_overwrite
                   ? JSProviderAnswer::kThirdPartyOverriding
                   : JSProviderAnswer::kThirdPartyNotOverriding;
      break;
    case mojom::JSProviderType::Native:
      if (is_wallet_setup) {
        // A native wallet is definitely not being overridden
        // if provider overwrites are allowed.
        answer = !allow_provider_overwrite
                     ? JSProviderAnswer::kNativeOverridingDisallowed
                     : JSProviderAnswer::kNativeNotOverridden;
      }
      break;
    default:
      NOTREACHED_NORETURN();
  }

  base::UmaHistogramEnumeration(histogram_name, answer);
}

absl::optional<mojom::OnboardingAction>
HnsWalletP3A::GetLastOnboardingAction() {
  if (local_state_->HasPrefPath(kHnsWalletP3AOnboardingLastStep)) {
    int pref_value =
        local_state_->GetInteger(kHnsWalletP3AOnboardingLastStep);
    return static_cast<mojom::OnboardingAction>(pref_value);
  }
  return absl::nullopt;
}

void HnsWalletP3A::ReportOnboardingAction(mojom::OnboardingAction action) {
  if (action == mojom::OnboardingAction::StartRestore) {
    // We do not want to monitor wallet restores; cancel the
    // histogram record timer and wipe out the last onboarding step.
    local_state_->ClearPref(kHnsWalletP3AOnboardingLastStep);
    onboarding_report_timer_.Stop();
    return;
  }
  absl::optional<mojom::OnboardingAction> last_step = GetLastOnboardingAction();
  if (!last_step.has_value() || *last_step < action) {
    // Only record steps that are ahead of the previous step so we
    // don't record back navigation.
    local_state_->SetInteger(kHnsWalletP3AOnboardingLastStep,
                             static_cast<int>(action));
  }
  if (onboarding_report_timer_.IsRunning() ||
      action == mojom::OnboardingAction::Shown) {
    // If the event is the first possible action (aka the shown event),
    // or if timer is already running (re)start the timer to debounce.
    onboarding_report_timer_.Start(
        FROM_HERE, kOnboardingRecordDelay,
        base::BindOnce(&HnsWalletP3A::RecordOnboardingHistogram,
                       base::Unretained(this)));
  } else {
    // If the timer is not running and the action is after the first possible
    // event, report it right away since it probably missed the 120 sec
    // deadline.
    RecordOnboardingHistogram();
  }
}

void HnsWalletP3A::RecordOnboardingHistogram() {
  absl::optional<mojom::OnboardingAction> last_step = GetLastOnboardingAction();
  if (!last_step.has_value()) {
    return;
  }
  local_state_->ClearPref(kHnsWalletP3AOnboardingLastStep);
  UMA_HISTOGRAM_ENUMERATION(kOnboardingConversionHistogramName, *last_step);
}

void HnsWalletP3A::ReportTransactionSent(mojom::CoinType coin,
                                           bool new_send) {
  const char* histogram_name = nullptr;
  switch (coin) {
    case mojom::CoinType::ETH:
      histogram_name = kEthTransactionSentHistogramName;
      break;
    case mojom::CoinType::SOL:
      histogram_name = kSolTransactionSentHistogramName;
      break;
    case mojom::CoinType::FIL:
      histogram_name = kFilTransactionSentHistogramName;
      break;
    case mojom::CoinType::BTC:
      // TODO(apaymyshev): https://github.com/hnsvn/hns-browser/issues/28464
      return;
    default:
      return;
  }

  DCHECK(histogram_name);

  ScopedDictPrefUpdate last_sent_time_update(
      profile_prefs_, kHnsWalletLastTransactionSentTimeDict);
  base::Value::Dict& last_sent_time_dict = last_sent_time_update.Get();

  std::string coin_key = base::NumberToString(static_cast<int>(coin));

  base::Time now = base::Time::Now();
  base::Time last_sent_time = base::Time::FromDoubleT(
      last_sent_time_dict.FindDouble(coin_key).value_or(0.0));

  if (!new_send && last_sent_time.is_null()) {
    // Don't report if a transaction was never sent.
    return;
  }
  int answer = 0;
  if (new_send || (now - last_sent_time) < base::Days(7)) {
    answer = 1;
  }
  if (new_send) {
    last_sent_time_dict.Set(coin_key, now.ToDoubleT());
  }

  base::UmaHistogramExactLinear(histogram_name, answer, 2);
}

void HnsWalletP3A::RecordActiveWalletCount(int count,
                                             mojom::CoinType coin_type) {
  DCHECK_GE(count, 0);
  const char* histogram_name = nullptr;

  switch (coin_type) {
    case mojom::CoinType::ETH:
      histogram_name = kEthActiveAccountHistogramName;
      break;
    case mojom::CoinType::SOL:
      histogram_name = kSolActiveAccountHistogramName;
      break;
    case mojom::CoinType::FIL:
      histogram_name = kFilActiveAccountHistogramName;
      break;
    case mojom::CoinType::BTC:
      // TODO(apaymyshev): https://github.com/hnsvn/hns-browser/issues/28464
      return;
    default:
      return;
  }

  DCHECK(histogram_name);

  const base::Value::Dict& active_wallet_dict =
      profile_prefs_->GetDict(kHnsWalletP3AActiveWalletDict);
  std::string coin_type_str = base::NumberToString(static_cast<int>(coin_type));
  if (!active_wallet_dict.FindBool(coin_type_str).has_value()) {
    if (count == 0) {
      // Should not record zero to histogram if user never had an active
      // account, to avoid sending unnecessary data.
      return;
    }
    ScopedDictPrefUpdate active_wallet_dict_update(
        profile_prefs_, kHnsWalletP3AActiveWalletDict);
    active_wallet_dict_update->Set(coin_type_str, true);
  }
  p3a_utils::RecordToHistogramBucket(histogram_name, kActiveAccountBuckets,
                                     count);

  if (count > 0) {
    MaybeRecordNewUserBalance();
  }
}

void HnsWalletP3A::RecordNFTGalleryView(int nft_count) {
  if (!local_state_->GetBoolean(kHnsWalletP3ANFTGalleryUsed)) {
    local_state_->SetBoolean(kHnsWalletP3ANFTGalleryUsed, true);
    UMA_HISTOGRAM_BOOLEAN(kHnsWalletNFTNewUserHistogramName, true);
  }
  p3a_utils::RecordToHistogramBucket(kHnsWalletNFTCountHistogramName,
                                     kNFTCountBuckets, nft_count);
}

void HnsWalletP3A::MaybeRecordNewUserBalance() {
  base::Time deadline = base::Time::Now() - base::Days(7);
  if (local_state_->GetTime(kHnsWalletP3AFirstUnlockTime) >= deadline &&
      !local_state_->GetBoolean(kHnsWalletP3ANewUserBalanceReported)) {
    UMA_HISTOGRAM_BOOLEAN(kNewUserBalanceHistogramName, true);
    local_state_->SetBoolean(kHnsWalletP3ANewUserBalanceReported, true);
  }
}

void HnsWalletP3A::ReportNftDiscoverySetting() {
  if (!local_state_->GetTime(kHnsWalletLastUnlockTime).is_null()) {
    UMA_HISTOGRAM_BOOLEAN(
        kHnsWalletNFTDiscoveryEnabledHistogramName,
        profile_prefs_->GetBoolean(kHnsWalletNftDiscoveryEnabled));
  }
}

// TODO(djandries): remove pref migration around November 2023
void HnsWalletP3A::MigrateUsageProfilePrefsToLocalState() {
  for (const char* pref_name : kTimePrefsToMigrateToLocalState) {
    if (local_state_->GetTime(pref_name).is_null()) {
      base::Time profile_time = profile_prefs_->GetTime(pref_name);
      if (!profile_time.is_null()) {
        local_state_->SetTime(pref_name, profile_time);
        profile_prefs_->ClearPref(pref_name);
      }
    }
  }
  for (const char* pref_name : kTimePrefsToRemove) {
    local_state_->ClearPref(pref_name);
    profile_prefs_->ClearPref(pref_name);
  }
  if (!local_state_->GetBoolean(kHnsWalletP3AUsedSecondDay)) {
    bool profile_used_second_day =
        profile_prefs_->GetBoolean(kHnsWalletP3AUsedSecondDay);
    if (profile_used_second_day) {
      local_state_->SetBoolean(kHnsWalletP3AUsedSecondDay, true);
      profile_prefs_->ClearPref(kHnsWalletP3AUsedSecondDay);
    }
  }
  local_state_->ClearPref(kHnsWalletP3AWeeklyStorage);
  profile_prefs_->ClearPref(kHnsWalletP3AWeeklyStorage);
}

void HnsWalletP3A::OnUpdateTimerFired() {
  ReportUsage(false);
  ReportTransactionSent(mojom::CoinType::ETH, false);
  ReportTransactionSent(mojom::CoinType::FIL, false);
  ReportTransactionSent(mojom::CoinType::SOL, false);
}

void HnsWalletP3A::WriteUsageStatsToHistogram() {
  VLOG(1) << "Wallet P3A: Recording usage";
  UMA_HISTOGRAM_BOOLEAN(kHnsWalletMonthlyHistogramName, true);
  UMA_HISTOGRAM_BOOLEAN(kHnsWalletWeeklyHistogramName, true);
  UMA_HISTOGRAM_BOOLEAN(kHnsWalletDailyHistogramName, true);
}

void HnsWalletP3A::RecordInitialHnsWalletP3AState() {
  keyring_service_->GetKeyringInfo(mojom::kDefaultKeyringId,
                                   base::BindOnce(&RecordKeyringCreated));
}

// KeyringServiceObserver
void HnsWalletP3A::KeyringCreated(mojom::KeyringId keyring_id) {
  keyring_service_->GetKeyringInfo(keyring_id,
                                   base::BindOnce(&RecordKeyringCreated));
}

}  // namespace hns_wallet
