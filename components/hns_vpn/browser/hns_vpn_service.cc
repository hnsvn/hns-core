/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/hns_vpn_service.h"

#include <algorithm>
#include <utility>

#include "base/base64.h"
#include "base/check_is_test.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/time/time.h"
#include "hns/components/hns_vpn/browser/api/hns_vpn_api_helper.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service_helper.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_region_data_manager.h"
#include "hns/components/hns_vpn/common/hns_vpn_constants.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"
#include "hns/components/p3a_utils/feature_usage.h"
#include "hns/components/skus/browser/skus_utils.h"
#include "hns/components/version_info/version_info.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_service.h"
#include "components/version_info/version_info.h"
#include "net/cookies/cookie_inclusion_status.h"
#include "net/cookies/cookie_util.h"
#include "net/cookies/parsed_cookie.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/l10n/l10n_util.h"
#include "url/url_util.h"

#if BUILDFLAG(IS_WIN)
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"
#endif

namespace hns_vpn {

using ConnectionState = mojom::ConnectionState;
using PurchasedState = mojom::PurchasedState;

HnsVpnService::HnsVpnService(
    HnsVPNOSConnectionAPI* connection_api,
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    PrefService* profile_prefs,
    base::RepeatingCallback<mojo::PendingRemote<skus::mojom::SkusService>()>
        skus_service_getter)
    : local_prefs_(local_prefs),
      profile_prefs_(profile_prefs),
      skus_service_getter_(skus_service_getter),
      api_request_(new HnsVpnAPIRequest(url_loader_factory)) {
  DCHECK(IsHnsVPNFeatureEnabled());
#if !BUILDFLAG(IS_ANDROID)
  DCHECK(connection_api);
  connection_api_ = connection_api;
  observed_.Observe(connection_api_);

  policy_pref_change_registrar_.Init(profile_prefs_);
  policy_pref_change_registrar_.Add(
      prefs::kManagedHnsVPNDisabled,
      base::BindRepeating(&HnsVpnService::OnPreferenceChanged,
                          base::Unretained(this)));

#endif  // !BUILDFLAG(IS_ANDROID)

  CheckInitialState();
  InitP3A();
}

HnsVpnService::~HnsVpnService() = default;

bool HnsVpnService::IsHnsVPNEnabled() const {
  return ::hns_vpn::IsHnsVPNEnabled(profile_prefs_);
}

void HnsVpnService::CheckInitialState() {
  if (HasValidSubscriberCredential(local_prefs_)) {
    ScheduleSubscriberCredentialRefresh();

#if BUILDFLAG(IS_ANDROID)
    SetPurchasedState(GetCurrentEnvironment(), PurchasedState::PURCHASED);
    // Android has its own region data managing logic.
#else
    if (connection_api_->GetRegionDataManager().IsRegionDataReady()) {
      SetPurchasedState(GetCurrentEnvironment(), PurchasedState::PURCHASED);
    } else {
      SetPurchasedState(GetCurrentEnvironment(), PurchasedState::LOADING);
      // Not sure this can happen when user is already purchased user.
      // To make sure before set as purchased user, however, trying region fetch
      // and then set as a purchased user after we get valid region data.
      wait_region_data_ready_ = true;
    }
    connection_api_->GetRegionDataManager().FetchRegionDataIfNeeded();
#endif
  } else if (HasValidSkusCredential(local_prefs_)) {
    // If we have valid skus creds during the startup, we can try to get subs
    // credential in advance.
    ReloadPurchasedState();
  } else {
    // Try to reload purchased state if cached credential is not valid because
    // it could be invalidated when not running.
    if (HasSubscriberCredential(local_prefs_)) {
      VLOG(2) << __func__ << " "
              << "Try to reload purchased as invalid credential is stored.";
      ClearSubscriberCredential(local_prefs_);
      ReloadPurchasedState();
    } else {
      ClearSubscriberCredential(local_prefs_);
    }
  }
}

std::string HnsVpnService::GetCurrentEnvironment() const {
  return local_prefs_->GetString(prefs::kHnsVPNEnvironment);
}

void HnsVpnService::ReloadPurchasedState() {
  LoadPurchasedState(skus::GetDomain("vpn", GetCurrentEnvironment()));
}

void HnsVpnService::BindInterface(
    mojo::PendingReceiver<mojom::ServiceHandler> receiver) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  receivers_.Add(this, std::move(receiver));
}

#if !BUILDFLAG(IS_ANDROID)
void HnsVpnService::OnConnectionStateChanged(mojom::ConnectionState state) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  VLOG(2) << __func__ << " " << state;
#if BUILDFLAG(IS_WIN)
  WriteConnectionState(static_cast<int>(state));
#endif
  // Ignore connection state change request for non purchased user.
  // This can be happened when user controls vpn via os settings.
  if (!is_purchased_user())
    return;

  if (state == ConnectionState::CONNECTED) {
    // If user connected vpn from the system and launched the browser
    // we detected it was disabled by policies and disabling it.
    if (IsHnsVPNDisabledByPolicy(profile_prefs_)) {
      connection_api_->Disconnect();
      return;
    }
#if BUILDFLAG(IS_WIN)
    // Run tray process each time we establish connection. System tray icon
    // manages self state to be visible/hidden due to settings.
    wireguard::ShowHnsVpnStatusTrayIcon();
#endif
    RecordP3A(true);
  }

  for (const auto& obs : observers_)
    obs->OnConnectionStateChanged(state);
}

void HnsVpnService::OnRegionDataReady(bool success) {
  VLOG(2) << __func__ << " success - " << success << ", is waiting? "
          << wait_region_data_ready_;
  if (!wait_region_data_ready_) {
    return;
  }

  wait_region_data_ready_ = false;

  // Happened weird state while waiting region data.
  // Don't update purchased if current state is not loading state.
  if (GetPurchasedInfoSync().state != PurchasedState::LOADING) {
    return;
  }

  SetPurchasedState(GetCurrentEnvironment(), success ? PurchasedState::PURCHASED
                                                     : PurchasedState::FAILED);
}

void HnsVpnService::OnSelectedRegionChanged(const std::string& region_name) {
  const auto region_ptr = GetRegionPtrWithNameFromRegionList(
      region_name, connection_api_->GetRegionDataManager().GetRegions());
  for (const auto& obs : observers_) {
    obs->OnSelectedRegionChanged(region_ptr.Clone());
  }
}

mojom::ConnectionState HnsVpnService::GetConnectionState() const {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  return connection_api_->GetConnectionState();
}

bool HnsVpnService::IsConnected() const {
  if (!is_purchased_user()) {
    return false;
  }

  return GetConnectionState() == ConnectionState::CONNECTED;
}

void HnsVpnService::Connect() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!is_purchased_user()) {
    return;
  }

  connection_api_->Connect();
}

void HnsVpnService::Disconnect() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!is_purchased_user()) {
    return;
  }

  connection_api_->Disconnect();
}

void HnsVpnService::ToggleConnection() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!is_purchased_user()) {
    return;
  }

  connection_api_->ToggleConnection();
}

void HnsVpnService::GetConnectionState(GetConnectionStateCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  const auto state = connection_api_->GetConnectionState();
  VLOG(2) << __func__ << " : " << state;
  std::move(callback).Run(state);
}

void HnsVpnService::GetAllRegions(GetAllRegionsCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  std::vector<mojom::RegionPtr> regions;
  for (const auto& region :
       connection_api_->GetRegionDataManager().GetRegions()) {
    regions.push_back(region.Clone());
  }
  std::move(callback).Run(std::move(regions));
}

void HnsVpnService::GetSelectedRegion(GetSelectedRegionCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  VLOG(2) << __func__;

  auto region_name =
      connection_api_->GetRegionDataManager().GetSelectedRegion();
  std::move(callback).Run(GetRegionPtrWithNameFromRegionList(
      region_name, connection_api_->GetRegionDataManager().GetRegions()));
}

void HnsVpnService::SetSelectedRegion(mojom::RegionPtr region_ptr) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  VLOG(2) << __func__ << " : " << region_ptr->name_pretty;
  connection_api_->SetSelectedRegion(region_ptr->name);
}

void HnsVpnService::GetProductUrls(GetProductUrlsCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  std::move(callback).Run(mojom::ProductUrls::New(
      kFeedbackUrl, kAboutUrl, GetManageUrl(GetCurrentEnvironment())));
}

void HnsVpnService::CreateSupportTicket(
    const std::string& email,
    const std::string& subject,
    const std::string& body,
    CreateSupportTicketCallback callback) {
  auto internal_callback =
      base::BindOnce(&HnsVpnService::OnCreateSupportTicket,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  api_request_->CreateSupportTicket(
      std::move(internal_callback), email, subject, body,
      ::hns_vpn::GetSubscriberCredential(local_prefs_));
}

void HnsVpnService::GetSupportData(GetSupportDataCallback callback) {
  std::string hns_version =
      version_info::GetHnsVersionWithoutChromiumMajorVersion();

  std::move(callback).Run(hns_version, std::string(version_info::GetOSType()),
                          connection_api_->GetHostname(), GetTimeZoneName());
}

void HnsVpnService::ResetConnectionState() {
  connection_api_->ResetConnectionState();
}

// NOTE(bsclifton): Desktop uses API to create a ticket.
// Android and iOS directly send an email.
void HnsVpnService::OnCreateSupportTicket(
    CreateSupportTicketCallback callback,
    const std::string& ticket,
    bool success) {
  std::move(callback).Run(success, ticket);
}

void HnsVpnService::OnPreferenceChanged(const std::string& pref_name) {
  if (pref_name == prefs::kManagedHnsVPNDisabled) {
    if (IsHnsVPNDisabledByPolicy(profile_prefs_)) {
      connection_api_->Disconnect();
    }
    return;
  }
}

void HnsVpnService::UpdatePurchasedStateForSessionExpired(
    const std::string& env) {
  // Double check that we don't set session expired state for fresh user.
  if (!connection_api_->GetRegionDataManager().IsRegionDataReady()) {
    VLOG(1) << __func__ << " : Treat it as not purchased state for fresh user.";
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
    return;
  }

  const auto session_expired_time =
      local_prefs_->GetTime(prefs::kHnsVPNSessionExpiredDate);
  // If it's not cached, it means this session expiration is first time since
  // last purchase because this cache is cleared when we get valid credential
  // summary.
  if (session_expired_time.is_null()) {
    local_prefs_->SetTime(prefs::kHnsVPNSessionExpiredDate,
                          base::Time::Now());
    SetPurchasedState(env, PurchasedState::SESSION_EXPIRED);
    return;
  }

  // Weird state. Maybe we don't see this condition.
  // Just checking for safe.
  if (session_expired_time > base::Time::Now()) {
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
    return;
  }

  // Keep session expired state 30 days at most.
  constexpr int kSessionExpiredCheckingDurationInDays = 30;
  if ((base::Time::Now() - session_expired_time).InDays() >
      kSessionExpiredCheckingDurationInDays) {
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
    return;
  }

  SetPurchasedState(env, PurchasedState::SESSION_EXPIRED);
}
#endif  // !BUILDFLAG(IS_ANDROID)

#if BUILDFLAG(IS_ANDROID)
void HnsVpnService::GetPurchaseToken(GetPurchaseTokenCallback callback) {
  std::string purchase_token_string = "";
  std::string package_string = "com.hns.browser";
  std::string product_id_string = "hns-firewall-vpn-premium";

  // Get the Android purchase token (for Google Play Store).
  // The value for this is validated on the account.hns.com side
  auto* purchase_token =
      profile_prefs_->FindPreference(prefs::kHnsVPNPurchaseTokenAndroid);
  if (purchase_token && !purchase_token->IsDefaultValue()) {
    purchase_token_string =
        profile_prefs_->GetString(prefs::kHnsVPNPurchaseTokenAndroid);
  }

  // Package name is important; for real users, it'll be the Release package.
  // For testing we do have the ability to use the Nightly package.
  auto* package =
      profile_prefs_->FindPreference(prefs::kHnsVPNPackageAndroid);
  if (package && !package->IsDefaultValue()) {
    package_string = profile_prefs_->GetString(prefs::kHnsVPNPackageAndroid);
  }

  auto* product_id =
      profile_prefs_->FindPreference(prefs::kHnsVPNProductIdAndroid);
  if (product_id && !product_id->IsDefaultValue()) {
    product_id_string =
        profile_prefs_->GetString(prefs::kHnsVPNProductIdAndroid);
  }

  base::Value::Dict response;
  response.Set("type", "android");
  response.Set("raw_receipt", purchase_token_string);
  response.Set("package", package_string);
  response.Set("subscription_id", product_id_string);

  std::string response_json;
  base::JSONWriter::Write(response, &response_json);

  std::string encoded_response_json;
  base::Base64Encode(response_json, &encoded_response_json);

  std::move(callback).Run(encoded_response_json);
}
#endif  // BUILDFLAG(IS_ANDROID)

void HnsVpnService::AddObserver(
    mojo::PendingRemote<mojom::ServiceObserver> observer) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  observers_.Add(std::move(observer));
}

mojom::PurchasedInfo HnsVpnService::GetPurchasedInfoSync() const {
  return purchased_state_.value_or(mojom::PurchasedInfo(
      mojom::PurchasedState::NOT_PURCHASED, absl::nullopt));
}

void HnsVpnService::GetPurchasedState(GetPurchasedStateCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  std::move(callback).Run(GetPurchasedInfoSync().Clone());
}

void HnsVpnService::LoadPurchasedState(const std::string& domain) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  auto requested_env = skus::GetEnvironmentForDomain(domain);
  if (GetCurrentEnvironment() == requested_env &&
      GetPurchasedInfoSync().state == PurchasedState::LOADING) {
    VLOG(2) << __func__ << ": Loading in-progress";
    return;
  }

  SetPurchasedState(requested_env, PurchasedState::LOADING);

  if (HasValidSubscriberCredential(local_prefs_)) {
#if BUILDFLAG(IS_ANDROID)
    SetPurchasedState(requested_env, PurchasedState::PURCHASED);
#else
    if (connection_api_->GetRegionDataManager().IsRegionDataReady()) {
      VLOG(2) << __func__
              << ": Set as a purchased user as we have valid subscriber "
                 "credentials & region data";
      SetPurchasedState(requested_env, PurchasedState::PURCHASED);
    } else {
      VLOG(2) << __func__ << ": Wait till we get valid region data.";
      // TODO(simonhong): Make purchases state independent from region data.
      wait_region_data_ready_ = true;
    }
    connection_api_->GetRegionDataManager().FetchRegionDataIfNeeded();
#endif
    return;
  }

  if (HasValidSkusCredential(local_prefs_)) {
    // We can reach here if we fail to get subscriber credential from guardian.
    VLOG(2) << __func__
            << " Try to get subscriber credential with valid cached skus "
               "credential.";

    if (GetCurrentEnvironment() != requested_env) {
      SetCurrentEnvironment(requested_env);
    }

    api_request_->GetSubscriberCredentialV12(
        base::BindOnce(&HnsVpnService::OnGetSubscriberCredentialV12,
                       base::Unretained(this),
                       GetExpirationTimeForSkusCredential(local_prefs_)),
        GetSkusCredential(local_prefs_),
        GetHnsVPNPaymentsEnv(GetCurrentEnvironment()));
    return;
  }

  VLOG(2) << __func__
          << ": Checking purchased state as we doesn't have valid skus or "
             "subscriber credentials";

  RequestCredentialSummary(domain);
}

void HnsVpnService::RequestCredentialSummary(const std::string& domain) {
  // As we request new credential, clear cached value.
  ClearSubscriberCredential(local_prefs_);

  EnsureMojoConnected();
  skus_service_->CredentialSummary(
      domain, base::BindOnce(&HnsVpnService::OnCredentialSummary,
                             base::Unretained(this), domain));
}

void HnsVpnService::OnCredentialSummary(const std::string& domain,
                                          const std::string& summary_string) {
  auto env = skus::GetEnvironmentForDomain(domain);
  std::string summary_string_trimmed;
  base::TrimWhitespaceASCII(summary_string, base::TrimPositions::TRIM_ALL,
                            &summary_string_trimmed);
  if (summary_string_trimmed.length() == 0) {
    // no credential found; person needs to login
    VLOG(1) << __func__ << " : No credential found; user needs to login!";
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
    return;
  }

  absl::optional<base::Value> records_v = base::JSONReader::Read(
      summary_string, base::JSONParserOptions::JSON_PARSE_RFC);

  // Early return when summary is invalid or it's empty dict.
  if (!records_v || !records_v->is_dict()) {
    VLOG(1) << __func__ << " : Got invalid credential summary!";
    SetPurchasedState(env, PurchasedState::FAILED);
    return;
  }

  // Empty dict - clean user.
  if (records_v->GetDict().empty()) {
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
    return;
  }

  if (IsValidCredentialSummary(*records_v)) {
    VLOG(1) << __func__ << " : Active credential found!";
    // if a credential is ready, we can present it
    EnsureMojoConnected();
    skus_service_->PrepareCredentialsPresentation(
        domain, "*",
        base::BindOnce(&HnsVpnService::OnPrepareCredentialsPresentation,
                       base::Unretained(this), domain));
#if !BUILDFLAG(IS_ANDROID)
    // Clear expired state data as we have active credentials.
    local_prefs_->SetTime(prefs::kHnsVPNSessionExpiredDate, {});
#endif
  } else if (IsValidCredentialSummaryButNeedActivation(*records_v)) {
    // Need to activate from account. Treat as not purchased till activated.
    VLOG(1) << __func__ << " : Need to activate vpn from account.";
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
  } else {
    // When reaches here, remained_credential is zero. We can treat it as
    // user's current purchase is expired.
    VLOG(1) << __func__ << " : don't have remained credential.";
#if BUILDFLAG(IS_ANDROID)
    VLOG(1) << __func__ << " : Treat it as not purchased state in android.";
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
#else
    VLOG(1) << __func__ << " : Treat it as session expired state in desktop.";
    UpdatePurchasedStateForSessionExpired(env);
#endif
  }
}

void HnsVpnService::OnPrepareCredentialsPresentation(
    const std::string& domain,
    const std::string& credential_as_cookie) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  auto env = skus::GetEnvironmentForDomain(domain);
  // Credential is returned in cookie format.
  net::CookieInclusionStatus status;
  net::ParsedCookie credential_cookie(credential_as_cookie,
                                      /*block_truncated=*/true, &status);
  // TODO(bsclifton): have a better check / logging.
  // should these failed states be considered NOT_PURCHASED?
  // or maybe it can be considered FAILED status?
  if (!credential_cookie.IsValid()) {
    VLOG(1) << __func__ << " : FAILED credential_cookie.IsValid";
    // TODO(simonhong): Set as NOT_PURCHASED.
    // It seems we're not using this state.
    SetPurchasedState(env, PurchasedState::FAILED);
    return;
  }
  if (!status.IsInclude()) {
    VLOG(1) << __func__ << " : FAILED status.IsInclude";
    SetPurchasedState(env, PurchasedState::FAILED);
    return;
  }

  if (!credential_cookie.HasExpires()) {
    VLOG(1) << __func__ << " : FAILED cookie doesn't have expired date.";
    SetPurchasedState(env, PurchasedState::FAILED);
    return;
  }

  // Credential value received needs to be URL decoded.
  // That leaves us with a Base64 encoded JSON blob which is the credential.
  const std::string encoded_credential = credential_cookie.Value();
  const auto time =
      net::cookie_util::ParseCookieExpirationTime(credential_cookie.Expires());
  url::RawCanonOutputT<char16_t> unescaped;
  url::DecodeURLEscapeSequences(
      encoded_credential.data(), encoded_credential.size(),
      url::DecodeURLMode::kUTF8OrIsomorphic, &unescaped);
  std::string credential;
  base::UTF16ToUTF8(unescaped.data(), unescaped.length(), &credential);
  if (credential.empty()) {
    SetPurchasedState(env, PurchasedState::NOT_PURCHASED);
    return;
  }

  // Early return when it's already expired.
  if (time < base::Time::Now()) {
    SetPurchasedState(
        GetCurrentEnvironment(), PurchasedState::FAILED,
        l10n_util::GetStringUTF8(IDS_HNS_VPN_PURCHASE_CREDENTIALS_EXPIRED));
    return;
  }

  SetSkusCredential(local_prefs_, credential, time);

  if (GetCurrentEnvironment() != env) {
    // Change environment because we have successfully authorized with new one.
    SetCurrentEnvironment(env);
  }

  api_request_->GetSubscriberCredentialV12(
      base::BindOnce(&HnsVpnService::OnGetSubscriberCredentialV12,
                     base::Unretained(this), time),
      credential, GetHnsVPNPaymentsEnv(GetCurrentEnvironment()));
}

void HnsVpnService::OnGetSubscriberCredentialV12(
    const base::Time& expiration_time,
    const std::string& subscriber_credential,
    bool success) {
  if (!success) {
    VLOG(2) << __func__ << " : failed to get subscriber credential";
#if BUILDFLAG(IS_ANDROID)
    SetPurchasedState(GetCurrentEnvironment(), PurchasedState::NOT_PURCHASED);
#else
    const bool token_no_longer_valid =
        subscriber_credential == kTokenNoLongerValid;

    // If current skus-credential is from retried, don't retry to get newer
    // skus-credential again.
    if (token_no_longer_valid && !IsRetriedSkusCredential(local_prefs_)) {
      VLOG(2) << __func__
              << " : Re-trying to fetch subscriber-credential by fetching "
                 "newer skus-credential.";
      RequestCredentialSummary(skus::GetDomain("vpn", GetCurrentEnvironment()));
      SetSkusCredentialFetchingRetried(local_prefs_, true);
      return;
    }

    // If we got same error with another skus-credential, give up as we can't
    // issue another skus-credential. It's limited resource.
    if (token_no_longer_valid && IsRetriedSkusCredential(local_prefs_)) {
      VLOG(2) << __func__
              << " : Got TokenNoLongerValid again with retried skus credential";
    }

    auto message_id = token_no_longer_valid
                          ? IDS_HNS_VPN_PURCHASE_TOKEN_NOT_VALID
                          : IDS_HNS_VPN_PURCHASE_CREDENTIALS_FETCH_FAILED;
    SetPurchasedState(GetCurrentEnvironment(), PurchasedState::FAILED,
                      l10n_util::GetStringUTF8(message_id));
#endif
    return;
  }

  // Clear retrying flags as we got valid subscriber-credential.
  SetSkusCredentialFetchingRetried(local_prefs_, false);

  // Previously cached skus credential is cleared and fetched subscriber
  // credential is cached.
  SetSubscriberCredential(local_prefs_, subscriber_credential, expiration_time);

  // Launch one-shot timer for refreshing subscriber_credential before it's
  // expired.
  ScheduleSubscriberCredentialRefresh();

#if BUILDFLAG(IS_ANDROID)
  SetPurchasedState(GetCurrentEnvironment(), PurchasedState::PURCHASED);
#else
  if (connection_api_->GetRegionDataManager().IsRegionDataReady()) {
    SetPurchasedState(GetCurrentEnvironment(), PurchasedState::PURCHASED);
  } else {
    wait_region_data_ready_ = true;
  }
  connection_api_->GetRegionDataManager().FetchRegionDataIfNeeded();
#endif
}

void HnsVpnService::ScheduleSubscriberCredentialRefresh() {
  if (subs_cred_refresh_timer_.IsRunning())
    subs_cred_refresh_timer_.Stop();

  const auto expiration_time = GetExpirationTime(local_prefs_);
  if (!expiration_time)
    return;

  auto expiration_time_delta = *expiration_time - base::Time::Now();
  VLOG(2) << "Schedule subscriber credential fetching after "
          << expiration_time_delta;
  subs_cred_refresh_timer_.Start(
      FROM_HERE, expiration_time_delta,
      base::BindOnce(&HnsVpnService::RefreshSubscriberCredential,
                     base::Unretained(this)));
}

void HnsVpnService::RefreshSubscriberCredential() {
  VLOG(2) << "Refresh subscriber credential";

  // Clear current credentials to get newer one.
  ClearSubscriberCredential(local_prefs_);
  ReloadPurchasedState();
}

// TODO(simonhong): Should move p3a to HnsVPNOSConnectionAPI?
void HnsVpnService::InitP3A() {
  p3a_timer_.Start(FROM_HERE, base::Hours(kP3AIntervalHours), this,
                   &HnsVpnService::OnP3AInterval);
  RecordP3A(false);
}

void HnsVpnService::RecordP3A(bool new_usage) {
  if (new_usage) {
    p3a_utils::RecordFeatureUsage(local_prefs_, prefs::kHnsVPNFirstUseTime,
                                  prefs::kHnsVPNLastUseTime);
  }
  p3a_utils::RecordFeatureNewUserReturning(
      local_prefs_, prefs::kHnsVPNFirstUseTime, prefs::kHnsVPNLastUseTime,
      prefs::kHnsVPNUsedSecondDay, kNewUserReturningHistogramName);
  p3a_utils::RecordFeatureDaysInMonthUsed(
      local_prefs_, new_usage, prefs::kHnsVPNLastUseTime,
      prefs::kHnsVPNDaysInMonthUsed, kDaysInMonthUsedHistogramName);
  p3a_utils::RecordFeatureLastUsageTimeMetric(
      local_prefs_, prefs::kHnsVPNLastUseTime, kLastUsageTimeHistogramName);
}

#if BUILDFLAG(IS_ANDROID)
void HnsVpnService::RecordAndroidBackgroundP3A(int64_t session_start_time_ms,
                                                 int64_t session_end_time_ms) {
  if (session_start_time_ms < 0 || session_end_time_ms < 0) {
    RecordP3A(false);
    return;
  }
  base::Time session_start_time =
      base::Time::FromJsTime(static_cast<double>(session_start_time_ms))
          .LocalMidnight();
  base::Time session_end_time =
      base::Time::FromJsTime(static_cast<double>(session_end_time_ms))
          .LocalMidnight();
  for (base::Time day = session_start_time; day <= session_end_time;
       day += base::Days(1)) {
    bool is_last_day = day == session_end_time;
    // Call functions for each day in the last session to ensure
    // p3a_util functions produce the correct result
    p3a_utils::RecordFeatureUsage(local_prefs_, prefs::kHnsVPNFirstUseTime,
                                  prefs::kHnsVPNLastUseTime, day);
    p3a_utils::RecordFeatureNewUserReturning(
        local_prefs_, prefs::kHnsVPNFirstUseTime, prefs::kHnsVPNLastUseTime,
        prefs::kHnsVPNUsedSecondDay, kNewUserReturningHistogramName,
        is_last_day);
    p3a_utils::RecordFeatureDaysInMonthUsed(
        local_prefs_, day, prefs::kHnsVPNLastUseTime,
        prefs::kHnsVPNDaysInMonthUsed, kDaysInMonthUsedHistogramName,
        is_last_day);
  }
  p3a_utils::RecordFeatureLastUsageTimeMetric(
      local_prefs_, prefs::kHnsVPNLastUseTime, kLastUsageTimeHistogramName);
}
#endif

void HnsVpnService::OnP3AInterval() {
  RecordP3A(false);
}

void HnsVpnService::SetPurchasedState(
    const std::string& env,
    PurchasedState state,
    const absl::optional<std::string>& description) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (GetPurchasedInfoSync().state == state || env != GetCurrentEnvironment()) {
    return;
  }

  VLOG(2) << __func__ << " : " << state;
  purchased_state_ = mojom::PurchasedInfo(state, description);

  for (const auto& obs : observers_)
    obs->OnPurchasedStateChanged(state, description);

#if !BUILDFLAG(IS_ANDROID)
  if (state == PurchasedState::PURCHASED)
    connection_api_->CheckConnection();
#endif
}

void HnsVpnService::SetCurrentEnvironment(const std::string& env) {
  local_prefs_->SetString(prefs::kHnsVPNEnvironment, env);
  purchased_state_.reset();
}

void HnsVpnService::EnsureMojoConnected() {
  if (!skus_service_) {
    auto pending = skus_service_getter_.Run();
    skus_service_.Bind(std::move(pending));
  }
  DCHECK(skus_service_);
  skus_service_.set_disconnect_handler(base::BindOnce(
      &HnsVpnService::OnMojoConnectionError, base::Unretained(this)));
}

void HnsVpnService::OnMojoConnectionError() {
  skus_service_.reset();
  EnsureMojoConnected();
}

void HnsVpnService::Shutdown() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  skus_service_.reset();
  observers_.Clear();
  api_request_.reset();
  p3a_timer_.Stop();
  subs_cred_refresh_timer_.Stop();

#if !BUILDFLAG(IS_ANDROID)
  observed_.Reset();
  receivers_.Clear();
#endif  // !BUILDFLAG(IS_ANDROID)
}

void HnsVpnService::GetAllServerRegions(ResponseCallback callback) {
  api_request_->GetAllServerRegions(std::move(callback));
}

void HnsVpnService::GetTimezonesForRegions(ResponseCallback callback) {
  api_request_->GetTimezonesForRegions(std::move(callback));
}

void HnsVpnService::GetHostnamesForRegion(ResponseCallback callback,
                                            const std::string& region) {
  api_request_->GetHostnamesForRegion(std::move(callback), region);
}

void HnsVpnService::GetProfileCredentials(
    ResponseCallback callback,
    const std::string& subscriber_credential,
    const std::string& hostname) {
  api_request_->GetProfileCredentials(std::move(callback),
                                      subscriber_credential, hostname);
}

void HnsVpnService::GetWireguardProfileCredentials(
    ResponseCallback callback,
    const std::string& subscriber_credential,
    const std::string& public_key,
    const std::string& hostname) {
  api_request_->GetWireguardProfileCredentials(
      std::move(callback), subscriber_credential, public_key, hostname);
}

void HnsVpnService::VerifyCredentials(
    ResponseCallback callback,
    const std::string& hostname,
    const std::string& client_id,
    const std::string& subscriber_credential,
    const std::string& api_auth_token) {
  api_request_->VerifyCredentials(std::move(callback), hostname, client_id,
                                  subscriber_credential, api_auth_token);
}

void HnsVpnService::InvalidateCredentials(
    ResponseCallback callback,
    const std::string& hostname,
    const std::string& client_id,
    const std::string& subscriber_credential,
    const std::string& api_auth_token) {
  api_request_->InvalidateCredentials(std::move(callback), hostname, client_id,
                                      subscriber_credential, api_auth_token);
}

void HnsVpnService::VerifyPurchaseToken(ResponseCallback callback,
                                          const std::string& purchase_token,
                                          const std::string& product_id,
                                          const std::string& product_type,
                                          const std::string& bundle_id) {
  api_request_->VerifyPurchaseToken(std::move(callback), purchase_token,
                                    product_id, product_type, bundle_id);
}

void HnsVpnService::GetSubscriberCredential(
    ResponseCallback callback,
    const std::string& product_type,
    const std::string& product_id,
    const std::string& validation_method,
    const std::string& purchase_token,
    const std::string& bundle_id) {
  api_request_->GetSubscriberCredential(std::move(callback), product_type,
                                        product_id, validation_method,
                                        purchase_token, bundle_id);
}

void HnsVpnService::GetSubscriberCredentialV12(ResponseCallback callback) {
  // Caller can get valid subscriber credential only in purchased state.
  // Otherwise, false is passed to |callback| as success param.
  std::move(callback).Run(::hns_vpn::GetSubscriberCredential(local_prefs_),
                          HasValidSubscriberCredential(local_prefs_));
}

}  // namespace hns_vpn
