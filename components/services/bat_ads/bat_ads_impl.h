/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SERVICES_BAT_ADS_BAT_ADS_IMPL_H_
#define HNS_COMPONENTS_SERVICES_BAT_ADS_BAT_ADS_IMPL_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "base/task/sequenced_task_runner.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "hns/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"

namespace hns_ads {
class Ads;
struct InlineContentAdInfo;
struct NewTabPageAdInfo;
}  // namespace hns_ads

namespace bat_ads {

class BatAdsClientMojoBridge;

class BatAdsImpl : public mojom::BatAds {
 public:
  BatAdsImpl(
      mojo::PendingAssociatedRemote<mojom::BatAdsClient> client,
      mojo::PendingReceiver<mojom::BatAdsClientNotifier> client_notifier);

  BatAdsImpl(const BatAdsImpl&) = delete;
  BatAdsImpl& operator=(const BatAdsImpl&) = delete;

  BatAdsImpl(BatAdsImpl&& other) noexcept = delete;
  BatAdsImpl& operator=(BatAdsImpl&& other) noexcept = delete;

  ~BatAdsImpl() override;

  // mojom::BatAds:
  void SetSysInfo(hns_ads::mojom::SysInfoPtr sys_info) override;
  void SetBuildChannel(
      hns_ads::mojom::BuildChannelInfoPtr build_channel) override;
  void SetFlags(hns_ads::mojom::FlagsPtr flags) override;

  void Initialize(hns_ads::mojom::WalletInfoPtr wallet,
                  InitializeCallback callback) override;
  void Shutdown(ShutdownCallback callback) override;

  void GetDiagnostics(GetDiagnosticsCallback callback) override;

  void GetStatementOfAccounts(GetStatementOfAccountsCallback callback) override;

  void MaybeServeInlineContentAd(
      const std::string& dimensions,
      MaybeServeInlineContentAdCallback callback) override;
  void TriggerInlineContentAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      hns_ads::mojom::InlineContentAdEventType event_type,
      TriggerInlineContentAdEventCallback callback) override;

  void MaybeServeNewTabPageAd(MaybeServeNewTabPageAdCallback callback) override;
  void TriggerNewTabPageAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      hns_ads::mojom::NewTabPageAdEventType event_type,
      TriggerNewTabPageAdEventCallback callback) override;

  void MaybeGetNotificationAd(const std::string& placement_id,
                              MaybeGetNotificationAdCallback callback) override;
  void TriggerNotificationAdEvent(
      const std::string& placement_id,
      hns_ads::mojom::NotificationAdEventType event_type,
      TriggerNotificationAdEventCallback callback) override;

  void TriggerPromotedContentAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      hns_ads::mojom::PromotedContentAdEventType event_type,
      TriggerPromotedContentAdEventCallback callback) override;

  void TriggerSearchResultAdEvent(
      hns_ads::mojom::SearchResultAdInfoPtr ad_mojom,
      hns_ads::mojom::SearchResultAdEventType event_type,
      TriggerSearchResultAdEventCallback callback) override;

  void PurgeOrphanedAdEventsForType(
      hns_ads::mojom::AdType ad_type,
      PurgeOrphanedAdEventsForTypeCallback callback) override;

  void GetHistory(base::Time from_time,
                  base::Time to_time,
                  GetHistoryCallback callback) override;

  void ToggleLikeAd(base::Value::Dict value,
                    ToggleLikeAdCallback callback) override;
  void ToggleDislikeAd(base::Value::Dict value,
                       ToggleLikeAdCallback callback) override;
  void ToggleLikeCategory(base::Value::Dict value,
                          ToggleLikeCategoryCallback callback) override;
  void ToggleDislikeCategory(base::Value::Dict value,
                             ToggleDislikeCategoryCallback callback) override;
  void ToggleSaveAd(base::Value::Dict value,
                    ToggleSaveAdCallback callback) override;
  void ToggleMarkAdAsInappropriate(
      base::Value::Dict value,
      ToggleMarkAdAsInappropriateCallback callback) override;

 private:
  hns_ads::Ads* GetAds();

  class AdsInstance;
  std::unique_ptr<AdsInstance, base::OnTaskRunnerDeleter> ads_instance_;
};

}  // namespace bat_ads

#endif  // HNS_COMPONENTS_SERVICES_BAT_ADS_BAT_ADS_IMPL_H_
