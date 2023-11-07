/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_rewards/android/hns_rewards_native_worker.h"

#include <iomanip>
#include <string>
#include <vector>
#include <utility>

#include "base/android/jni_android.h"
#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
#include "base/containers/contains.h"
#include "base/containers/flat_map.h"
#include "base/json/json_writer.h"
#include "base/strings/stringprintf.h"
#include "base/time/time.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/build/android/jni_headers/HnsRewardsNativeWorker_jni.h"
#include "hns/components/hns_adaptive_captcha/hns_adaptive_captcha_service.h"
#include "hns/components/hns_adaptive_captcha/server_util.h"
#include "hns/components/hns_ads/browser/ads_service.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/url_data_source.h"

#define DEFAULT_ADS_PER_HOUR 2

namespace chrome {
namespace android {

HnsRewardsNativeWorker::HnsRewardsNativeWorker(JNIEnv* env,
    const base::android::JavaRef<jobject>& obj):
    weak_java_hns_rewards_native_worker_(env, obj),
    hns_rewards_service_(nullptr),
    weak_factory_(this) {
  Java_HnsRewardsNativeWorker_setNativePtr(env, obj,
    reinterpret_cast<intptr_t>(this));

  hns_rewards_service_ = hns_rewards::RewardsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (hns_rewards_service_) {
    hns_rewards_service_->AddObserver(this);
    hns_rewards::RewardsNotificationService* notification_service =
      hns_rewards_service_->GetNotificationService();
    if (notification_service) {
      notification_service->AddObserver(this);
    }
  }
}

HnsRewardsNativeWorker::~HnsRewardsNativeWorker() {
}

void HnsRewardsNativeWorker::Destroy(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->RemoveObserver(this);
    hns_rewards::RewardsNotificationService* notification_service =
      hns_rewards_service_->GetNotificationService();
    if (notification_service) {
      notification_service->RemoveObserver(this);
    }
  }
  delete this;
}

bool HnsRewardsNativeWorker::IsSupported(JNIEnv* env) {
  return hns_rewards::IsSupported(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile()->GetPrefs(),
      hns_rewards::IsSupportedOptions::kNone);
}

bool HnsRewardsNativeWorker::IsSupportedSkipRegionCheck(JNIEnv* env) {
  return hns_rewards::IsSupported(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile()->GetPrefs(),
      hns_rewards::IsSupportedOptions::kSkipRegionCheck);
}

std::string HnsRewardsNativeWorker::StringifyResult(
    hns_rewards::mojom::CreateRewardsWalletResult result) {
  switch (result) {
    case hns_rewards::mojom::CreateRewardsWalletResult::kSuccess:
      return "success";
    case hns_rewards::mojom::CreateRewardsWalletResult::
        kWalletGenerationDisabled:
      return "wallet-generation-disabled";
    case hns_rewards::mojom::CreateRewardsWalletResult::
        kGeoCountryAlreadyDeclared:
      return "country-already-declared";
    case hns_rewards::mojom::CreateRewardsWalletResult::kUnexpected:
      return "unexpected-error";
  }
}

bool HnsRewardsNativeWorker::IsRewardsEnabled(JNIEnv* env) {
  return ProfileManager::GetActiveUserProfile()
      ->GetOriginalProfile()
      ->GetPrefs()
      ->GetBoolean(hns_rewards::prefs::kEnabled);
}

void HnsRewardsNativeWorker::CreateRewardsWallet(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& country_code) {
  if (hns_rewards_service_) {
    hns_rewards_service_->CreateRewardsWallet(
        base::android::ConvertJavaStringToUTF8(env, country_code),
        base::BindOnce(&HnsRewardsNativeWorker::OnCreateRewardsWallet,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnCreateRewardsWallet(
    hns_rewards::mojom::CreateRewardsWalletResult result) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onCreateRewardsWallet(
      env, weak_java_hns_rewards_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, StringifyResult(result)));
}

void HnsRewardsNativeWorker::GetRewardsParameters(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetRewardsParameters(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetRewardsParameters,
                       weak_factory_.GetWeakPtr(), hns_rewards_service_));
  }
}

void HnsRewardsNativeWorker::OnGetRewardsParameters(
    hns_rewards::RewardsService* rewards_service,
    hns_rewards::mojom::RewardsParametersPtr parameters) {
  if (parameters) {
    parameters_ = std::move(parameters);
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnRewardsParameters(
      env, weak_java_hns_rewards_native_worker_.get(env));
}

double HnsRewardsNativeWorker::GetVbatDeadline(JNIEnv* env) {
  if (parameters_) {
    if (!parameters_->vbat_deadline.is_null()) {
      return floor(parameters_->vbat_deadline.ToDoubleT() *
                   base::Time::kMillisecondsPerSecond);
    }
  }
  return 0.0;
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetPayoutStatus(JNIEnv* env) {
  std::string wallet_type;
  std::string payout_status;
  if (hns_rewards_service_) {
    wallet_type = hns_rewards_service_->GetExternalWalletType();
    if (parameters_) {
      if (!parameters_->payout_status.empty()) {
        payout_status = parameters_->payout_status.at(wallet_type);
      }
    }
  }
  return base::android::ConvertUTF8ToJavaString(env, payout_status);
}

void HnsRewardsNativeWorker::GetUserType(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetUserType(base::BindOnce(
        &HnsRewardsNativeWorker::OnGetUserType, weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnGetUserType(
    const hns_rewards::mojom::UserType user_type) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onGetUserType(
      env, weak_java_hns_rewards_native_worker_.get(env),
      static_cast<int>(user_type));
}

bool HnsRewardsNativeWorker::IsGrandfatheredUser(JNIEnv* env) {
  bool is_grandfathered_user = false;
  if (hns_rewards_service_) {
    is_grandfathered_user = hns_rewards_service_->IsGrandfatheredUser();
  }
  return is_grandfathered_user;
}

void HnsRewardsNativeWorker::FetchBalance(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->FetchBalance(base::BindOnce(
        &HnsRewardsNativeWorker::OnBalance, weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnBalance(
    base::expected<hns_rewards::mojom::BalancePtr,
                   hns_rewards::mojom::FetchBalanceError> result) {
  if (result.has_value()) {
    balance_ = *result.value();
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onBalance(
      env, weak_java_hns_rewards_native_worker_.get(env), result.has_value());
}

void HnsRewardsNativeWorker::GetPublisherInfo(
    JNIEnv* env,
    int tabId,
    const base::android::JavaParamRef<jstring>& host) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetPublisherActivityFromUrl(tabId,
      base::android::ConvertJavaStringToUTF8(env, host), "", "");
  }
}

void HnsRewardsNativeWorker::OnPanelPublisherInfo(
    hns_rewards::RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    const hns_rewards::mojom::PublisherInfo* info,
    uint64_t tabId) {
  if (!info) {
    return;
  }
  hns_rewards::mojom::PublisherInfoPtr pi = info->Clone();
  map_publishers_info_[tabId] = std::move(pi);
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnPublisherInfo(env,
        weak_java_hns_rewards_native_worker_.get(env), tabId);
}

void HnsRewardsNativeWorker::OnUnblindedTokensReady(
    hns_rewards::RewardsService* rewards_service) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onUnblindedTokensReady(
      env, weak_java_hns_rewards_native_worker_.get(env));
}

void HnsRewardsNativeWorker::OnReconcileComplete(
    hns_rewards::RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    const std::string& contribution_id,
    const double amount,
    const hns_rewards::mojom::RewardsType type,
    const hns_rewards::mojom::ContributionProcessor processor) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onReconcileComplete(
      env, weak_java_hns_rewards_native_worker_.get(env),
      static_cast<int>(result), static_cast<int>(type), amount);
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetPublisherURL(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->url);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetPublisherFavIconURL(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env,
      iter->second->favicon_url);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetCaptchaSolutionURL(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& paymentId,
    const base::android::JavaParamRef<jstring>& captchaId) {
  const std::string path = base::StringPrintf(
      "/v3/captcha/solution/%s/%s",
      base::android::ConvertJavaStringToUTF8(env, paymentId).c_str(),
      base::android::ConvertJavaStringToUTF8(env, captchaId).c_str());
  std::string captcha_solution_url =
      hns_adaptive_captcha::ServerUtil::GetInstance()->GetServerUrl(path);

  return base::android::ConvertUTF8ToJavaString(env, captcha_solution_url);
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetAttestationURL(JNIEnv* env) {
  const std::string path = "/v1/attestations/android";
  std::string captcha_solution_url =
      hns_adaptive_captcha::ServerUtil::GetInstance()->GetServerUrl(path);

  return base::android::ConvertUTF8ToJavaString(env, captcha_solution_url);
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetAttestationURLWithPaymentId(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& paymentId) {
  const std::string path = base::StringPrintf(
      "/v1/attestations/android/%s",
      base::android::ConvertJavaStringToUTF8(env, paymentId).c_str());
  std::string captcha_solution_url =
      hns_adaptive_captcha::ServerUtil::GetInstance()->GetServerUrl(path);

  return base::android::ConvertUTF8ToJavaString(env, captcha_solution_url);
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetPublisherName(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->name);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetPublisherId(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->id);
  }

  return res;
}

int HnsRewardsNativeWorker::GetPublisherPercent(JNIEnv* env, uint64_t tabId) {
  int res = 0;

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = iter->second->percent;
  }

  return res;
}

bool HnsRewardsNativeWorker::GetPublisherExcluded(JNIEnv* env,
                                                    uint64_t tabId) {
  bool res = false;

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = iter->second->excluded ==
          hns_rewards::mojom::PublisherExclude::EXCLUDED;
  }

  return res;
}

int HnsRewardsNativeWorker::GetPublisherStatus(JNIEnv* env, uint64_t tabId) {
  int res =
      static_cast<int>(hns_rewards::mojom::PublisherStatus::NOT_VERIFIED);
  PublishersInfoMap::const_iterator iter = map_publishers_info_.find(tabId);
  if (iter != map_publishers_info_.end()) {
    res = static_cast<int>(iter->second->status);
  }
  return res;
}

void HnsRewardsNativeWorker::IncludeInAutoContribution(JNIEnv* env,
                                                         uint64_t tabId,
                                                         bool exclude) {
  PublishersInfoMap::iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    if (exclude) {
      iter->second->excluded = hns_rewards::mojom::PublisherExclude::EXCLUDED;
    } else {
      iter->second->excluded = hns_rewards::mojom::PublisherExclude::INCLUDED;
    }
    if (hns_rewards_service_) {
      hns_rewards_service_->SetPublisherExclude(iter->second->id, exclude);
    }
  }
}

void HnsRewardsNativeWorker::RemovePublisherFromMap(JNIEnv* env,
                                                      uint64_t tabId) {
  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    map_publishers_info_.erase(iter);
  }
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetWalletBalance(JNIEnv* env) {
  std::string json_balance;
  base::Value::Dict root;
  root.Set("total", balance_.total);

  base::Value::Dict json_wallets;
  for (const auto & item : balance_.wallets) {
    json_wallets.Set(item.first, item.second);
  }
  root.SetByDottedPath("wallets", std::move(json_wallets));
  base::JSONWriter::Write(std::move(root), &json_balance);

  return base::android::ConvertUTF8ToJavaString(env, json_balance);
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetExternalWalletType(JNIEnv* env) {
  std::string wallet_type;
  if (hns_rewards_service_) {
    wallet_type = hns_rewards_service_->GetExternalWalletType();
  }

  return base::android::ConvertUTF8ToJavaString(env, wallet_type);
}

void HnsRewardsNativeWorker::GetAdsAccountStatement(JNIEnv* env) {
  auto* ads_service = hns_ads::AdsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (!ads_service) {
    return;
  }
  ads_service->GetStatementOfAccounts(
      base::BindOnce(&HnsRewardsNativeWorker::OnGetAdsAccountStatement,
                     weak_factory_.GetWeakPtr()));
}

void HnsRewardsNativeWorker::OnGetAdsAccountStatement(
    hns_ads::mojom::StatementInfoPtr statement) {
  JNIEnv* env = base::android::AttachCurrentThread();
  if (!statement) {
    Java_HnsRewardsNativeWorker_OnGetAdsAccountStatement(
        env, weak_java_hns_rewards_native_worker_.get(env),
        /* success */ false, 0.0, 0, 0.0, 0.0, 0.0, 0.0);
    return;
  }

  Java_HnsRewardsNativeWorker_OnGetAdsAccountStatement(
      env, weak_java_hns_rewards_native_worker_.get(env),
      /* success */ true, statement->next_payment_date.ToDoubleT() * 1000,
      statement->ads_received_this_month, statement->min_earnings_this_month,
      statement->max_earnings_this_month, statement->min_earnings_last_month,
      statement->max_earnings_last_month);
}

bool HnsRewardsNativeWorker::CanConnectAccount(JNIEnv* env) {
  if (!parameters_ || !hns_rewards_service_) {
    return true;
  }
  std::string country_code = hns_rewards_service_->GetCountryCode();
  return base::ranges::any_of(
      hns_rewards_service_->GetExternalWalletProviders(),
      [this, &country_code](const std::string& provider) {
        if (!parameters_->wallet_provider_regions.count(provider)) {
          return true;
        }

        const auto& regions = parameters_->wallet_provider_regions.at(provider);
        if (!regions) {
          return true;
        }

        const auto& [allow, block] = *regions;
        if (allow.empty() && block.empty()) {
          return true;
        }

        return base::Contains(allow, country_code) ||
               (!block.empty() && !base::Contains(block, country_code));
      });
}

base::android::ScopedJavaLocalRef<jdoubleArray>
HnsRewardsNativeWorker::GetTipChoices(JNIEnv* env) {
  return base::android::ToJavaDoubleArray(
      env, parameters_ ? parameters_->tip_choices : std::vector<double>());
}

double HnsRewardsNativeWorker::GetWalletRate(JNIEnv* env) {
  return parameters_ ? parameters_->rate : 0.0;
}

void HnsRewardsNativeWorker::FetchGrants(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->FetchPromotions(base::DoNothing());
  }
}

void HnsRewardsNativeWorker::GetCurrentBalanceReport(JNIEnv* env) {
  if (hns_rewards_service_) {
    auto now = base::Time::Now();
    base::Time::Exploded exploded;
    now.LocalExplode(&exploded);

    hns_rewards_service_->GetBalanceReport(
        exploded.month, exploded.year,
        base::BindOnce(&HnsRewardsNativeWorker::OnGetCurrentBalanceReport,
                       weak_factory_.GetWeakPtr(), hns_rewards_service_));
  }
}

void HnsRewardsNativeWorker::OnGetCurrentBalanceReport(
    hns_rewards::RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    hns_rewards::mojom::BalanceReportInfoPtr report) {
  base::android::ScopedJavaLocalRef<jdoubleArray> java_array;
  JNIEnv* env = base::android::AttachCurrentThread();
  if (report) {
    std::vector<double> values;
    values.push_back(report->grants);
    values.push_back(report->earning_from_ads);
    values.push_back(report->auto_contribute);
    values.push_back(report->recurring_donation);
    values.push_back(report->one_time_donation);
    java_array = base::android::ToJavaDoubleArray(env, values);
  }
  Java_HnsRewardsNativeWorker_OnGetCurrentBalanceReport(env,
        weak_java_hns_rewards_native_worker_.get(env), java_array);
}

void HnsRewardsNativeWorker::Donate(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher_key,
    double amount,
    bool recurring) {
  if (hns_rewards_service_) {
    hns_rewards_service_->SendContribution(
        base::android::ConvertJavaStringToUTF8(env, publisher_key), amount,
        recurring,
        base::BindOnce(&HnsRewardsNativeWorker::OnSendContribution,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnSendContribution(bool result) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onSendContribution(
      env, weak_java_hns_rewards_native_worker_.get(env), result);
}

void HnsRewardsNativeWorker::GetAllNotifications(JNIEnv* env) {
  if (!hns_rewards_service_) {
    return;
  }
  hns_rewards::RewardsNotificationService* notification_service =
    hns_rewards_service_->GetNotificationService();
  if (notification_service) {
    notification_service->GetNotifications();
  }
}

void HnsRewardsNativeWorker::DeleteNotification(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& notification_id) {
  if (!hns_rewards_service_) {
    return;
  }
  hns_rewards::RewardsNotificationService* notification_service =
    hns_rewards_service_->GetNotificationService();
  if (notification_service) {
    notification_service->DeleteNotification(
      base::android::ConvertJavaStringToUTF8(env, notification_id));
  }
}

void HnsRewardsNativeWorker::GetGrant(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& promotionId) {
  if (hns_rewards_service_) {
    std::string promotion_id =
      base::android::ConvertJavaStringToUTF8(env, promotionId);
    hns_rewards_service_->ClaimPromotion(
        promotion_id,
        base::BindOnce(&HnsRewardsNativeWorker::OnClaimPromotion,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnClaimPromotion(
    const hns_rewards::mojom::Result result,
    hns_rewards::mojom::PromotionPtr promotion) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnClaimPromotion(env,
      weak_java_hns_rewards_native_worker_.get(env),
      static_cast<int>(result));
}

base::android::ScopedJavaLocalRef<jobjectArray>
    HnsRewardsNativeWorker::GetCurrentGrant(JNIEnv* env,
      int position) {
  if ((size_t)position > promotions_.size() - 1) {
    return base::android::ScopedJavaLocalRef<jobjectArray>();
  }
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2) <<
      (promotions_[position])->approximate_value;
  std::vector<std::string> values;
  values.push_back(stream.str());
  values.push_back(
    std::to_string((promotions_[position])->expires_at));
  values.push_back(
      std::to_string(static_cast<int>((promotions_[position])->type)));

  return base::android::ToJavaArrayOfStrings(env, values);
}

void HnsRewardsNativeWorker::GetRecurringDonations(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetRecurringTips(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetRecurringTips,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnGetRecurringTips(
    std::vector<hns_rewards::mojom::PublisherInfoPtr> list) {
  map_recurrent_publishers_.clear();
  for (const auto& item : list) {
    map_recurrent_publishers_[item->id] = item->Clone();
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnRecurringDonationUpdated(env,
        weak_java_hns_rewards_native_worker_.get(env));
}

bool HnsRewardsNativeWorker::IsCurrentPublisherInRecurrentDonations(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  return map_recurrent_publishers_.find(
    base::android::ConvertJavaStringToUTF8(env, publisher)) !=
      map_recurrent_publishers_.end();
}

void HnsRewardsNativeWorker::GetAutoContributeProperties(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetAutoContributeProperties(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetAutoContributeProperties,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnGetAutoContributeProperties(
    hns_rewards::mojom::AutoContributePropertiesPtr properties) {
  if (properties) {
    auto_contrib_properties_ = std::move(properties);
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnGetAutoContributeProperties(
      env, weak_java_hns_rewards_native_worker_.get(env));
}

bool HnsRewardsNativeWorker::IsAutoContributeEnabled(JNIEnv* env) {
  if (!auto_contrib_properties_) {
    return false;
  }

  return auto_contrib_properties_->enabled_contribute;
}

void HnsRewardsNativeWorker::GetReconcileStamp(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetReconcileStamp(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetGetReconcileStamp,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::ResetTheWholeState(JNIEnv* env) {
  if (!hns_rewards_service_) {
    OnResetTheWholeState(false);
    return;
  }
  hns_rewards_service_->CompleteReset(
      base::BindOnce(&HnsRewardsNativeWorker::OnResetTheWholeState,
                     weak_factory_.GetWeakPtr()));
}

void HnsRewardsNativeWorker::OnCompleteReset(const bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_HnsRewardsNativeWorker_onCompleteReset(
      env, weak_java_hns_rewards_native_worker_.get(env), success);
}

void HnsRewardsNativeWorker::OnResetTheWholeState(const bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_HnsRewardsNativeWorker_OnResetTheWholeState(env,
          weak_java_hns_rewards_native_worker_.get(env), success);
}

double HnsRewardsNativeWorker::GetPublisherRecurrentDonationAmount(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  double amount(0.0);
  auto it = map_recurrent_publishers_.find(
    base::android::ConvertJavaStringToUTF8(env, publisher));

  if (it != map_recurrent_publishers_.end()) {
    amount = it->second->weight;
  }
  return  amount;
}

void HnsRewardsNativeWorker::RemoveRecurring(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  if (hns_rewards_service_) {
      hns_rewards_service_->RemoveRecurringTip(
        base::android::ConvertJavaStringToUTF8(env, publisher));
      auto it = map_recurrent_publishers_.find(
          base::android::ConvertJavaStringToUTF8(env, publisher));

      if (it != map_recurrent_publishers_.end()) {
        map_recurrent_publishers_.erase(it);
      }
  }
}

void HnsRewardsNativeWorker::OnGetGetReconcileStamp(uint64_t timestamp) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_HnsRewardsNativeWorker_OnGetReconcileStamp(env,
          weak_java_hns_rewards_native_worker_.get(env), timestamp);
}

void HnsRewardsNativeWorker::OnNotificationAdded(
    hns_rewards::RewardsNotificationService* rewards_notification_service,
    const hns_rewards::RewardsNotificationService::RewardsNotification&
      notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_HnsRewardsNativeWorker_OnNotificationAdded(env,
        weak_java_hns_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, notification.id_),
        notification.type_,
        notification.timestamp_,
        base::android::ToJavaArrayOfStrings(env, notification.args_));
}

void HnsRewardsNativeWorker::OnGetAllNotifications(
    hns_rewards::RewardsNotificationService* rewards_notification_service,
    const hns_rewards::RewardsNotificationService::RewardsNotificationsList&
      notifications_list) {
  JNIEnv* env = base::android::AttachCurrentThread();

  // Notify about notifications count
  Java_HnsRewardsNativeWorker_OnNotificationsCount(env,
        weak_java_hns_rewards_native_worker_.get(env),
        notifications_list.size());

  hns_rewards::RewardsNotificationService::RewardsNotificationsList::
    const_iterator iter =
      std::max_element(notifications_list.begin(), notifications_list.end(),
        [](const hns_rewards::RewardsNotificationService::
            RewardsNotification& notification_a,
          const hns_rewards::RewardsNotificationService::
            RewardsNotification& notification_b) {
        return notification_a.timestamp_ > notification_b.timestamp_;
      });

  if (iter != notifications_list.end()) {
    Java_HnsRewardsNativeWorker_OnGetLatestNotification(env,
        weak_java_hns_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, iter->id_),
        iter->type_,
        iter->timestamp_,
        base::android::ToJavaArrayOfStrings(env, iter->args_));
  }
}

void HnsRewardsNativeWorker::OnNotificationDeleted(
      hns_rewards::RewardsNotificationService* rewards_notification_service,
      const hns_rewards::RewardsNotificationService::RewardsNotification&
        notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_HnsRewardsNativeWorker_OnNotificationDeleted(env,
        weak_java_hns_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, notification.id_));
}

void HnsRewardsNativeWorker::OnPromotionFinished(
    hns_rewards::RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    hns_rewards::mojom::PromotionPtr promotion) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_HnsRewardsNativeWorker_OnGrantFinish(env,
        weak_java_hns_rewards_native_worker_.get(env),
        static_cast<int>(result));
}

int HnsRewardsNativeWorker::GetAdsPerHour(JNIEnv* env) {
  auto* ads_service_ = hns_ads::AdsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (!ads_service_) {
    return DEFAULT_ADS_PER_HOUR;
  }
  return ads_service_->GetMaximumNotificationAdsPerHour();
}

void HnsRewardsNativeWorker::SetAdsPerHour(JNIEnv* env, jint value) {
  ProfileManager::GetActiveUserProfile()
      ->GetOriginalProfile()
      ->GetPrefs()
      ->SetInt64(hns_ads::prefs::kMaximumNotificationAdsPerHour,
                 static_cast<int64_t>(value));
}

void HnsRewardsNativeWorker::SetAutoContributionAmount(JNIEnv* env,
                                                         jdouble value) {
  if (hns_rewards_service_) {
    hns_rewards_service_->SetAutoContributionAmount(value);
  }
}

void HnsRewardsNativeWorker::GetAutoContributionAmount(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetAutoContributionAmount(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetAutoContributionAmount,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnGetAutoContributionAmount(
    double auto_contribution_amount) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnGetAutoContributionAmount(
      env, weak_java_hns_rewards_native_worker_.get(env),
      auto_contribution_amount);
}

void HnsRewardsNativeWorker::GetExternalWallet(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetExternalWallet(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetExternalWallet,
                       weak_factory_.GetWeakPtr()));
  }
}

base::android::ScopedJavaLocalRef<jstring>
HnsRewardsNativeWorker::GetCountryCode(JNIEnv* env) {
  std::string country_code;
  if (hns_rewards_service_) {
    country_code = hns_rewards_service_->GetCountryCode();
  }

  return base::android::ConvertUTF8ToJavaString(env, country_code);
}

void HnsRewardsNativeWorker::GetAvailableCountries(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetAvailableCountries(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetAvailableCountries,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnGetAvailableCountries(
    std::vector<std::string> countries) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onGetAvailableCountries(
      env, weak_java_hns_rewards_native_worker_.get(env),
      base::android::ToJavaArrayOfStrings(env, countries));
}

void HnsRewardsNativeWorker::GetPublishersVisitedCount(JNIEnv* env) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetPublishersVisitedCount(
        base::BindOnce(&HnsRewardsNativeWorker::OnGetPublishersVisitedCount,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::OnGetPublishersVisitedCount(int count) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onGetPublishersVisitedCount(
      env, weak_java_hns_rewards_native_worker_.get(env), count);
}

void HnsRewardsNativeWorker::GetPublisherBanner(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher_key) {
  if (hns_rewards_service_) {
    hns_rewards_service_->GetPublisherBanner(
        base::android::ConvertJavaStringToUTF8(env, publisher_key),
        base::BindOnce(&HnsRewardsNativeWorker::onPublisherBanner,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsRewardsNativeWorker::onPublisherBanner(
    hns_rewards::mojom::PublisherBannerPtr banner) {
  std::string json_banner_info;
  if (!banner) {
    json_banner_info = "";
  } else {
    base::Value::Dict dict;
    dict.Set("publisher_key", banner->publisher_key);
    dict.Set("title", banner->title);

    dict.Set("name", banner->name);
    dict.Set("description", banner->description);
    dict.Set("background", banner->background);
    dict.Set("logo", banner->logo);
    dict.Set("provider", banner->provider);
    dict.Set("web3_url", banner->web3_url);

    base::Value::Dict links;
    for (auto const& link : banner->links) {
      links.Set(link.first, link.second);
    }
    dict.Set("links", std::move(links));

    dict.Set("status", static_cast<int32_t>(banner->status));
    base::JSONWriter::Write(dict, &json_banner_info);
  }
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_onPublisherBanner(
      env, weak_java_hns_rewards_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, json_banner_info));
}

void HnsRewardsNativeWorker::OnGetExternalWallet(
    base::expected<hns_rewards::mojom::ExternalWalletPtr,
                   hns_rewards::mojom::GetExternalWalletError> result) {
  auto wallet = std::move(result).value_or(nullptr);
  std::string json_wallet;
  if (!wallet) {
    json_wallet = "";
  } else {
    base::Value::Dict dict;
    dict.Set("token", wallet->token);
    dict.Set("address", wallet->address);

    // enum class WalletStatus : int32_t
    dict.Set("status", static_cast<int32_t>(wallet->status));
    dict.Set("type", wallet->type);
    dict.Set("user_name", wallet->user_name);
    dict.Set("account_url", wallet->account_url);
    dict.Set("login_url", wallet->login_url);
    base::JSONWriter::Write(dict, &json_wallet);
  }
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnGetExternalWallet(
      env, weak_java_hns_rewards_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, json_wallet));
}

void HnsRewardsNativeWorker::DisconnectWallet(JNIEnv* env) {
  // TODO(zenparsing): Remove disconnect ability from Android UI.
}

void HnsRewardsNativeWorker::OnExternalWalletConnected() {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnExternalWalletConnected(
      env, weak_java_hns_rewards_native_worker_.get(env));
}

void HnsRewardsNativeWorker::OnExternalWalletLoggedOut() {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnExternalWalletLoggedOut(
      env, weak_java_hns_rewards_native_worker_.get(env));
}

void HnsRewardsNativeWorker::OnExternalWalletReconnected() {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnExternalWalletReconnected(
      env, weak_java_hns_rewards_native_worker_.get(env));
}

std::string HnsRewardsNativeWorker::StdStrStrMapToJsonString(
    const base::flat_map<std::string, std::string>& args) {
    std::string json_args;
    base::Value::Dict dict;
    for (const auto & item : args) {
      dict.Set(item.first, item.second);
    }
    base::JSONWriter::Write(dict, &json_args);
    return json_args;
}

void HnsRewardsNativeWorker::RefreshPublisher(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher_key) {
  if (!hns_rewards_service_) {
    NOTREACHED();
    return;
  }
  hns_rewards_service_->RefreshPublisher(
      base::android::ConvertJavaStringToUTF8(env, publisher_key),
      base::BindOnce(&HnsRewardsNativeWorker::OnRefreshPublisher,
                     weak_factory_.GetWeakPtr()));
}

void HnsRewardsNativeWorker::OnRefreshPublisher(
    const hns_rewards::mojom::PublisherStatus status,
    const std::string& publisher_key) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsRewardsNativeWorker_OnRefreshPublisher(
      env, weak_java_hns_rewards_native_worker_.get(env),
      static_cast<int>(status),
      base::android::ConvertUTF8ToJavaString(env, publisher_key));
}

void HnsRewardsNativeWorker::SetAutoContributeEnabled(
    JNIEnv* env,
    bool isAutoContributeEnabled) {
  if (hns_rewards_service_) {
    hns_rewards_service_->SetAutoContributeEnabled(isAutoContributeEnabled);
  }
}

static void JNI_HnsRewardsNativeWorker_Init(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  new HnsRewardsNativeWorker(env, jcaller);
}

}  // namespace android
}  // namespace chrome
