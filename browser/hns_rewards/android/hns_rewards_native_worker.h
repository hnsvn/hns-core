/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_REWARDS_ANDROID_HNS_REWARDS_NATIVE_WORKER_H_
#define HNS_BROWSER_HNS_REWARDS_ANDROID_HNS_REWARDS_NATIVE_WORKER_H_

#include <jni.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/android/jni_weak_ref.h"
#include "base/containers/flat_map.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/types/expected.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "hns/components/hns_rewards/browser/rewards_notification_service_observer.h"
#include "hns/components/hns_rewards/browser/rewards_service_observer.h"
#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards {
class RewardsService;
}

namespace chrome {
namespace android {

typedef std::map<uint64_t, hns_rewards::mojom::PublisherInfoPtr>
    PublishersInfoMap;

class HnsRewardsNativeWorker
    : public hns_rewards::RewardsServiceObserver,
      public hns_rewards::RewardsNotificationServiceObserver {
 public:
  HnsRewardsNativeWorker(JNIEnv* env,
                           const base::android::JavaRef<jobject>& obj);
  ~HnsRewardsNativeWorker() override;

  void Destroy(JNIEnv* env);

  bool IsSupported(JNIEnv* env);
  bool IsSupportedSkipRegionCheck(JNIEnv* env);

  std::string StringifyResult(
      hns_rewards::mojom::CreateRewardsWalletResult result);

  bool IsRewardsEnabled(JNIEnv* env);

  void CreateRewardsWallet(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& country_code);

  void GetRewardsParameters(JNIEnv* env);

  double GetVbatDeadline(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetPayoutStatus(JNIEnv* env);

  void GetUserType(JNIEnv* env);

  bool IsGrandfatheredUser(JNIEnv* env);

  void FetchBalance(JNIEnv* env);

  void GetPublisherInfo(JNIEnv* env,
                        int tabId,
                        const base::android::JavaParamRef<jstring>& host);

  base::android::ScopedJavaLocalRef<jstring> GetWalletBalance(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetExternalWalletType(JNIEnv* env);

  void GetAdsAccountStatement(JNIEnv* env);

  bool CanConnectAccount(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jdoubleArray> GetTipChoices(JNIEnv* env);

  double GetWalletRate(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetPublisherURL(JNIEnv* env,
                                                             uint64_t tabId);

  base::android::ScopedJavaLocalRef<jstring> GetPublisherFavIconURL(
      JNIEnv* env,
      uint64_t tabId);

  base::android::ScopedJavaLocalRef<jstring> GetPublisherName(JNIEnv* env,
                                                              uint64_t tabId);
  base::android::ScopedJavaLocalRef<jstring> GetCaptchaSolutionURL(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& paymentId,
      const base::android::JavaParamRef<jstring>& captchaId);
  base::android::ScopedJavaLocalRef<jstring> GetAttestationURL(JNIEnv* env);
  base::android::ScopedJavaLocalRef<jstring> GetAttestationURLWithPaymentId(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& paymentId);

  base::android::ScopedJavaLocalRef<jstring> GetPublisherId(JNIEnv* env,
                                                            uint64_t tabId);

  int GetPublisherPercent(JNIEnv* env, uint64_t tabId);

  bool GetPublisherExcluded(JNIEnv* env, uint64_t tabId);

  int GetPublisherStatus(JNIEnv* env, uint64_t tabId);

  void GetCurrentBalanceReport(JNIEnv* env);

  void IncludeInAutoContribution(JNIEnv* env, uint64_t tabId, bool exclude);

  void RemovePublisherFromMap(JNIEnv* env, uint64_t tabId);

  void Donate(JNIEnv* env,
              const base::android::JavaParamRef<jstring>& publisher_key,
              double amount,
              bool recurring);

  void GetAllNotifications(JNIEnv* env);

  void DeleteNotification(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& notification_id);

  void GetGrant(JNIEnv* env,
                const base::android::JavaParamRef<jstring>& promotionId);

  base::android::ScopedJavaLocalRef<jobjectArray> GetCurrentGrant(JNIEnv* env,
                                                                  int position);

  void GetRecurringDonations(JNIEnv* env);

  bool IsCurrentPublisherInRecurrentDonations(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& publisher);

  void GetAutoContributeProperties(JNIEnv* env);

  bool IsAutoContributeEnabled(JNIEnv* env);

  void GetReconcileStamp(JNIEnv* env);

  void ResetTheWholeState(JNIEnv* env);

  double GetPublisherRecurrentDonationAmount(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& publisher);

  void RemoveRecurring(JNIEnv* env,
                       const base::android::JavaParamRef<jstring>& publisher);

  void FetchGrants(JNIEnv* env);

  int GetAdsPerHour(JNIEnv* env);

  void SetAdsPerHour(JNIEnv* env, jint value);

  void SetAutoContributionAmount(JNIEnv* env, jdouble value);

  void GetAutoContributionAmount(JNIEnv* env);

  void GetExternalWallet(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetCountryCode(JNIEnv* env);

  void GetAvailableCountries(JNIEnv* env);

  void GetPublisherBanner(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& publisher_key);

  void GetPublishersVisitedCount(JNIEnv* env);

  void OnGetPublishersVisitedCount(int count);

  void DisconnectWallet(JNIEnv* env);

  void RefreshPublisher(
      JNIEnv* env,
      const base::android::JavaParamRef<jstring>& publisher_key);

  void OnCreateRewardsWallet(
      hns_rewards::mojom::CreateRewardsWalletResult result);

  void OnCompleteReset(const bool success) override;

  void OnResetTheWholeState(const bool success);

  void OnGetGetReconcileStamp(uint64_t timestamp);

  void OnGetAutoContributeProperties(
      hns_rewards::mojom::AutoContributePropertiesPtr properties);

  void OnGetAutoContributionAmount(double auto_contribution_amount);

  void OnPanelPublisherInfo(hns_rewards::RewardsService* rewards_service,
                            const hns_rewards::mojom::Result result,
                            const hns_rewards::mojom::PublisherInfo* info,
                            uint64_t tabId) override;

  void OnGetCurrentBalanceReport(
      hns_rewards::RewardsService* rewards_service,
      const hns_rewards::mojom::Result result,
      hns_rewards::mojom::BalanceReportInfoPtr report);

  void OnGetRewardsParameters(
      hns_rewards::RewardsService* rewards_service,
      hns_rewards::mojom::RewardsParametersPtr parameters);

  void OnUnblindedTokensReady(
      hns_rewards::RewardsService* rewards_service) override;

  void OnReconcileComplete(
      hns_rewards::RewardsService* rewards_service,
      const hns_rewards::mojom::Result result,
      const std::string& contribution_id,
      const double amount,
      const hns_rewards::mojom::RewardsType type,
      const hns_rewards::mojom::ContributionProcessor processor) override;

  void OnNotificationAdded(
      hns_rewards::RewardsNotificationService* rewards_notification_service,
      const hns_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;

  void OnGetAllNotifications(
      hns_rewards::RewardsNotificationService* rewards_notification_service,
      const hns_rewards::RewardsNotificationService::RewardsNotificationsList&
          notifications_list) override;

  void OnNotificationDeleted(
      hns_rewards::RewardsNotificationService* rewards_notification_service,
      const hns_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;

  void OnPromotionFinished(
      hns_rewards::RewardsService* rewards_service,
      const hns_rewards::mojom::Result result,
      hns_rewards::mojom::PromotionPtr promotion) override;

  void OnGetRecurringTips(
      std::vector<hns_rewards::mojom::PublisherInfoPtr> list);

  void OnClaimPromotion(const hns_rewards::mojom::Result result,
                        hns_rewards::mojom::PromotionPtr promotion);

  void OnGetExternalWallet(
      base::expected<hns_rewards::mojom::ExternalWalletPtr,
                     hns_rewards::mojom::GetExternalWalletError> result);

  void OnGetAvailableCountries(std::vector<std::string> countries);

  void onPublisherBanner(hns_rewards::mojom::PublisherBannerPtr wallet);

  void OnSendContribution(bool result);

  void OnExternalWalletConnected() override;

  void OnExternalWalletLoggedOut() override;

  void OnExternalWalletReconnected() override;

  void OnRefreshPublisher(const hns_rewards::mojom::PublisherStatus status,
                          const std::string& publisher_key);
  void SetAutoContributeEnabled(JNIEnv* env, bool isAutoContributeEnabled);

 private:
  std::string StdStrStrMapToJsonString(
      const base::flat_map<std::string, std::string>& args);

  void OnGetUserType(const hns_rewards::mojom::UserType user_type);

  void OnBalance(
      base::expected<hns_rewards::mojom::BalancePtr,
                     hns_rewards::mojom::FetchBalanceError> result);

  void OnGetAdsAccountStatement(hns_ads::mojom::StatementInfoPtr statement);

  JavaObjectWeakGlobalRef weak_java_hns_rewards_native_worker_;
  raw_ptr<hns_rewards::RewardsService> hns_rewards_service_ = nullptr;
  hns_rewards::mojom::RewardsParametersPtr parameters_;
  hns_rewards::mojom::Balance balance_;
  hns_rewards::mojom::AutoContributePropertiesPtr auto_contrib_properties_;
  PublishersInfoMap map_publishers_info_;
  std::map<std::string, hns_rewards::mojom::PublisherInfoPtr>
      map_recurrent_publishers_;
  std::map<std::string, std::string> addresses_;
  std::vector<hns_rewards::mojom::PromotionPtr> promotions_;
  base::WeakPtrFactory<HnsRewardsNativeWorker> weak_factory_;
};

}  // namespace android
}  // namespace chrome

#endif  // HNS_BROWSER_HNS_REWARDS_ANDROID_HNS_REWARDS_NATIVE_WORKER_H_
