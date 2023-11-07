/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_API_HNS_REWARDS_API_H_
#define HNS_BROWSER_EXTENSIONS_API_HNS_REWARDS_API_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/types/expected.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class HnsRewardsIsSupportedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.isSupported", UNKNOWN)

 protected:
  ~HnsRewardsIsSupportedFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsIsUnsupportedRegionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.isUnsupportedRegion", UNKNOWN)

 protected:
  ~HnsRewardsIsUnsupportedRegionFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsRecordNTPPanelTriggerFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.recordNTPPanelTrigger", UNKNOWN)

 protected:
  ~HnsRewardsRecordNTPPanelTriggerFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsOpenRewardsPanelFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.openRewardsPanel", UNKNOWN)

 protected:
  ~HnsRewardsOpenRewardsPanelFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsShowRewardsSetupFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.showRewardsSetup", UNKNOWN)

 protected:
  ~HnsRewardsShowRewardsSetupFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsShowGrantCaptchaFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.showGrantCaptcha", UNKNOWN)

 protected:
  ~HnsRewardsShowGrantCaptchaFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsUpdateMediaDurationFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.updateMediaDuration", UNKNOWN)

 protected:
  ~HnsRewardsUpdateMediaDurationFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetPublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getPublisherInfo", UNKNOWN)

 protected:
  ~HnsRewardsGetPublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherInfo(const hns_rewards::mojom::Result result,
                          hns_rewards::mojom::PublisherInfoPtr info);
};

class HnsRewardsSetPublisherIdForTabFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.setPublisherIdForTab", UNKNOWN)

 protected:
  ~HnsRewardsSetPublisherIdForTabFunction() override;
  ResponseAction Run() override;
};

class HnsRewardsGetPublisherInfoForTabFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getPublisherInfoForTab", UNKNOWN)

 protected:
  ~HnsRewardsGetPublisherInfoForTabFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherPanelInfo(hns_rewards::mojom::Result result,
                               hns_rewards::mojom::PublisherInfoPtr info);
};

class HnsRewardsGetPublisherPanelInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getPublisherPanelInfo", UNKNOWN)

 protected:
  ~HnsRewardsGetPublisherPanelInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherPanelInfo(const hns_rewards::mojom::Result result,
                               hns_rewards::mojom::PublisherInfoPtr info);
};

class HnsRewardsSavePublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.savePublisherInfo", UNKNOWN)

 protected:
  ~HnsRewardsSavePublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnSavePublisherInfo(const hns_rewards::mojom::Result result);
};

class HnsRewardsTipSiteFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.tipSite", UNKNOWN)

 protected:
  ~HnsRewardsTipSiteFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsTipUserFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.tipUser", UNKNOWN)

 protected:
  ~HnsRewardsTipUserFunction() override;

  ResponseAction Run() override;

 private:
  void OnTipUserGetPublisherInfo(const hns_rewards::mojom::Result result,
                                 hns_rewards::mojom::PublisherInfoPtr info);
  void OnTipUserSavePublisherInfo(const hns_rewards::mojom::Result result);
  void ShowTipDialog();
};

class HnsRewardsGetPublisherDataFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getPublisherData", UNKNOWN)

 protected:
  ~HnsRewardsGetPublisherDataFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetRewardsParametersFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getRewardsParameters", UNKNOWN)

 protected:
  ~HnsRewardsGetRewardsParametersFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetRewardsParameters(
      hns_rewards::mojom::RewardsParametersPtr parameters);
};

class HnsRewardsCreateRewardsWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.createRewardsWallet", UNKNOWN)

 protected:
  ~HnsRewardsCreateRewardsWalletFunction() override;

  ResponseAction Run() override;

 private:
  void CreateRewardsWalletCallback(
      hns_rewards::mojom::CreateRewardsWalletResult result);
};

class HnsRewardsGetAvailableCountriesFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getAvailableCountries", UNKNOWN)

 protected:
  ~HnsRewardsGetAvailableCountriesFunction() override;

 private:
  void GetAvailableCountriesCallback(std::vector<std::string> countries);

  ResponseAction Run() override;
};

class HnsRewardsGetDefaultCountryFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getDefaultCountry", UNKNOWN)

 protected:
  ~HnsRewardsGetDefaultCountryFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetDeclaredCountryFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getDeclaredCountry", UNKNOWN)
 protected:
  ~HnsRewardsGetDeclaredCountryFunction() override;
  ResponseAction Run() override;
};

class HnsRewardsIsGrandfatheredUserFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.isGrandfatheredUser", UNKNOWN)

 protected:
  ~HnsRewardsIsGrandfatheredUserFunction() override;
  ResponseAction Run() override;
};

class HnsRewardsGetUserTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getUserType", UNKNOWN)

 protected:
  ~HnsRewardsGetUserTypeFunction() override;
  ResponseAction Run() override;

 private:
  void Callback(hns_rewards::mojom::UserType user_type);
};

class HnsRewardsGetPublishersVisitedCountFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getPublishersVisitedCount", UNKNOWN)

 protected:
  ~HnsRewardsGetPublishersVisitedCountFunction() override;

  ResponseAction Run() override;

 private:
  void Callback(int count);
};

class HnsRewardsGetBalanceReportFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getBalanceReport", UNKNOWN)

 protected:
  ~HnsRewardsGetBalanceReportFunction() override;

  ResponseAction Run() override;

 private:
  void OnBalanceReport(const hns_rewards::mojom::Result result,
                       hns_rewards::mojom::BalanceReportInfoPtr report);
};

class HnsRewardsIncludeInAutoContributionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.includeInAutoContribution", UNKNOWN)

 protected:
  ~HnsRewardsIncludeInAutoContributionFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsFetchPromotionsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.fetchPromotions", UNKNOWN)

 protected:
  ~HnsRewardsFetchPromotionsFunction() override;

  ResponseAction Run() override;

 private:
  void OnPromotionsFetched(
      std::vector<hns_rewards::mojom::PromotionPtr> promotions);
};

class HnsRewardsClaimPromotionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.claimPromotion", UNKNOWN)

 protected:
  ~HnsRewardsClaimPromotionFunction() override;

  ResponseAction Run() override;

 private:
  void OnClaimPromotion(const std::string& promotion_id,
                        const hns_rewards::mojom::Result result,
                        const std::string& captcha_image,
                        const std::string& hint,
                        const std::string& captcha_id);
};

class HnsRewardsAttestPromotionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.attestPromotion", UNKNOWN)

 protected:
  ~HnsRewardsAttestPromotionFunction() override;

  ResponseAction Run() override;

 private:
  void OnAttestPromotion(const std::string& promotion_id,
                         const hns_rewards::mojom::Result result,
                         hns_rewards::mojom::PromotionPtr promotion);
};

class HnsRewardsSetAutoContributeEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.setAutoContributeEnabled", UNKNOWN)

 protected:
  ~HnsRewardsSetAutoContributeEnabledFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetACEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getACEnabled", UNKNOWN)

 protected:
  ~HnsRewardsGetACEnabledFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetACEnabled(bool enabled);
};

class HnsRewardsSaveRecurringTipFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.saveRecurringTip", UNKNOWN)

 protected:
  ~HnsRewardsSaveRecurringTipFunction() override;

  ResponseAction Run() override;

 private:
  void OnSaveRecurringTip(hns_rewards::mojom::Result result);
};

class HnsRewardsRemoveRecurringTipFunction :
  public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.removeRecurringTip", UNKNOWN)

 protected:
  ~HnsRewardsRemoveRecurringTipFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetRecurringTipsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getRecurringTips", UNKNOWN)

 protected:
  ~HnsRewardsGetRecurringTipsFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetRecurringTips(
      std::vector<hns_rewards::mojom::PublisherInfoPtr> list);
};

class HnsRewardsRefreshPublisherFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.refreshPublisher", UNKNOWN)

 protected:
  ~HnsRewardsRefreshPublisherFunction() override;

  ResponseAction Run() override;

 private:
  void OnRefreshPublisher(const hns_rewards::mojom::PublisherStatus status,
                          const std::string& publisher_key);
};

class HnsRewardsGetAllNotificationsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getAllNotifications", UNKNOWN)

 protected:
  ~HnsRewardsGetAllNotificationsFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetInlineTippingPlatformEnabledFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "hnsRewards.getInlineTippingPlatformEnabled",
      UNKNOWN)

 protected:
  ~HnsRewardsGetInlineTippingPlatformEnabledFunction() override;

  ResponseAction Run() override;

 private:
  void OnInlineTipSetting(bool value);
};

class HnsRewardsFetchBalanceFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.fetchBalance", UNKNOWN)

 protected:
  ~HnsRewardsFetchBalanceFunction() override;

  ResponseAction Run() override;

 private:
  void OnFetchBalance(
      base::expected<hns_rewards::mojom::BalancePtr,
                     hns_rewards::mojom::FetchBalanceError> result);
};

class HnsRewardsGetExternalWalletProvidersFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getExternalWalletProviders", UNKNOWN)

 protected:
  ~HnsRewardsGetExternalWalletProvidersFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetExternalWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getExternalWallet", UNKNOWN)

 protected:
  ~HnsRewardsGetExternalWalletFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetExternalWallet(
      base::expected<hns_rewards::mojom::ExternalWalletPtr,
                     hns_rewards::mojom::GetExternalWalletError> result);
};

class HnsRewardsGetRewardsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getRewardsEnabled", UNKNOWN)

 protected:
  ~HnsRewardsGetRewardsEnabledFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetAdsAccountStatementFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getAdsAccountStatement", UNKNOWN)

 protected:
  ~HnsRewardsGetAdsAccountStatementFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetAdsAccountStatement(hns_ads::mojom::StatementInfoPtr statement);
};

class HnsRewardsIsInitializedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.isInitialized", UNKNOWN)

 protected:
  ~HnsRewardsIsInitializedFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetScheduledCaptchaInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getScheduledCaptchaInfo", UNKNOWN)

 protected:
  ~HnsRewardsGetScheduledCaptchaInfoFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsUpdateScheduledCaptchaResultFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.updateScheduledCaptchaResult",
                             UNKNOWN)

 protected:
  ~HnsRewardsUpdateScheduledCaptchaResultFunction() override;

  ResponseAction Run() override;
};

class HnsRewardsGetPrefsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsRewards.getPrefs", UNKNOWN)

 protected:
  ~HnsRewardsGetPrefsFunction() override;

  ResponseAction Run() override;

 private:
  void GetAutoContributePropertiesCallback(
      hns_rewards::mojom::AutoContributePropertiesPtr properties);
};

}  // namespace api
}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_API_HNS_REWARDS_API_H_
