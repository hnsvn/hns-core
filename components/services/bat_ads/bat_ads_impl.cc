/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/services/bat_ads/bat_ads_impl.h"

#include <utility>

#include "base/check.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"  // IWYU pragma: keep
#include "hns/components/hns_ads/core/public/ads.h"
#include "hns/components/hns_ads/core/public/ads/inline_content_ad_info.h"
#include "hns/components/hns_ads/core/public/ads/inline_content_ad_value_util.h"
#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_info.h"
#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_value_util.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_value_util.h"
#include "hns/components/hns_ads/core/public/history/ad_content_info.h"
#include "hns/components/hns_ads/core/public/history/ad_content_value_util.h"
#include "hns/components/hns_ads/core/public/history/category_content_info.h"
#include "hns/components/hns_ads/core/public/history/category_content_value_util.h"
#include "hns/components/hns_ads/core/public/history/history_filter_types.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"
#include "hns/components/hns_ads/core/public/history/history_item_value_util.h"
#include "hns/components/hns_ads/core/public/history/history_sort_types.h"
#include "hns/components/services/bat_ads/bat_ads_client_mojo_bridge.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace bat_ads {

class BatAdsImpl::AdsInstance final {
 public:
  AdsInstance(
      mojo::PendingAssociatedRemote<mojom::BatAdsClient> client,
      mojo::PendingReceiver<mojom::BatAdsClientNotifier> client_notifier)
      : bat_ads_client_mojo_proxy_(std::make_unique<BatAdsClientMojoBridge>(
            std::move(client),
            std::move(client_notifier))),
        ads_(hns_ads::Ads::CreateInstance(bat_ads_client_mojo_proxy_.get())) {
  }

  AdsInstance(const AdsInstance&) = delete;
  AdsInstance& operator=(const AdsInstance&) = delete;

  AdsInstance(AdsInstance&& other) noexcept = delete;
  AdsInstance& operator=(AdsInstance&& other) noexcept = delete;

  ~AdsInstance() = default;

  hns_ads::Ads* GetAds() { return ads_.get(); }

 private:
  std::unique_ptr<BatAdsClientMojoBridge> bat_ads_client_mojo_proxy_;
  std::unique_ptr<hns_ads::Ads> ads_;
};

BatAdsImpl::BatAdsImpl(
    mojo::PendingAssociatedRemote<mojom::BatAdsClient> client,
    mojo::PendingReceiver<mojom::BatAdsClientNotifier> client_notifier)
    : ads_instance_(std::unique_ptr<AdsInstance, base::OnTaskRunnerDeleter>(
          new AdsInstance(std::move(client), std::move(client_notifier)),
          base::OnTaskRunnerDeleter(
              base::SequencedTaskRunner::GetCurrentDefault()))) {}

BatAdsImpl::~BatAdsImpl() = default;

void BatAdsImpl::SetSysInfo(hns_ads::mojom::SysInfoPtr sys_info) {
  GetAds()->SetSysInfo(std::move(sys_info));
}

void BatAdsImpl::SetBuildChannel(
    hns_ads::mojom::BuildChannelInfoPtr build_channel) {
  GetAds()->SetBuildChannel(std::move(build_channel));
}

void BatAdsImpl::SetFlags(hns_ads::mojom::FlagsPtr flags) {
  GetAds()->SetFlags(std::move(flags));
}

void BatAdsImpl::Initialize(hns_ads::mojom::WalletInfoPtr wallet,
                            InitializeCallback callback) {
  GetAds()->Initialize(std::move(wallet), std::move(callback));
}

void BatAdsImpl::Shutdown(ShutdownCallback callback) {
  GetAds()->Shutdown(std::move(callback));
}

void BatAdsImpl::MaybeGetNotificationAd(
    const std::string& placement_id,
    MaybeGetNotificationAdCallback callback) {
  const absl::optional<hns_ads::NotificationAdInfo> ad =
      GetAds()->MaybeGetNotificationAd(placement_id);
  if (!ad) {
    std::move(callback).Run(/*ad*/ absl::nullopt);
    return;
  }

  absl::optional<base::Value::Dict> dict =
      hns_ads::NotificationAdToValue(*ad);
  std::move(callback).Run(std::move(dict));
}

void BatAdsImpl::TriggerNotificationAdEvent(
    const std::string& placement_id,
    const hns_ads::mojom::NotificationAdEventType event_type,
    TriggerNotificationAdEventCallback callback) {
  DCHECK(hns_ads::mojom::IsKnownEnumValue(event_type));

  GetAds()->TriggerNotificationAdEvent(placement_id, event_type,
                                       std::move(callback));
}

void BatAdsImpl::MaybeServeNewTabPageAd(
    MaybeServeNewTabPageAdCallback callback) {
  GetAds()->MaybeServeNewTabPageAd(base::BindOnce(
      [](MaybeServeNewTabPageAdCallback callback,
         const absl::optional<hns_ads::NewTabPageAdInfo>& ad) {
        if (!ad) {
          std::move(callback).Run(/*ad*/ absl::nullopt);
          return;
        }

        absl::optional<base::Value::Dict> dict =
            hns_ads::NewTabPageAdToValue(*ad);
        std::move(callback).Run(std::move(dict));
      },
      std::move(callback)));
}

void BatAdsImpl::TriggerNewTabPageAdEvent(
    const std::string& placement_id,
    const std::string& creative_instance_id,
    const hns_ads::mojom::NewTabPageAdEventType event_type,
    TriggerNewTabPageAdEventCallback callback) {
  DCHECK(hns_ads::mojom::IsKnownEnumValue(event_type));

  GetAds()->TriggerNewTabPageAdEvent(placement_id, creative_instance_id,
                                     event_type, std::move(callback));
}

void BatAdsImpl::TriggerPromotedContentAdEvent(
    const std::string& placement_id,
    const std::string& creative_instance_id,
    const hns_ads::mojom::PromotedContentAdEventType event_type,
    TriggerPromotedContentAdEventCallback callback) {
  DCHECK(hns_ads::mojom::IsKnownEnumValue(event_type));

  GetAds()->TriggerPromotedContentAdEvent(placement_id, creative_instance_id,
                                          event_type, std::move(callback));
}

void BatAdsImpl::MaybeServeInlineContentAd(
    const std::string& dimensions,
    MaybeServeInlineContentAdCallback callback) {
  GetAds()->MaybeServeInlineContentAd(
      dimensions,
      base::BindOnce(
          [](MaybeServeInlineContentAdCallback callback,
             const std::string& dimensions,
             const absl::optional<hns_ads::InlineContentAdInfo>& ad) {
            if (!ad) {
              std::move(callback).Run(dimensions,
                                      /*ads*/ absl::nullopt);
              return;
            }

            absl::optional<base::Value::Dict> dict =
                hns_ads::InlineContentAdToValue(*ad);
            std::move(callback).Run(dimensions, std::move(dict));
          },
          std::move(callback)));
}

void BatAdsImpl::TriggerInlineContentAdEvent(
    const std::string& placement_id,
    const std::string& creative_instance_id,
    const hns_ads::mojom::InlineContentAdEventType event_type,
    TriggerInlineContentAdEventCallback callback) {
  DCHECK(hns_ads::mojom::IsKnownEnumValue(event_type));

  GetAds()->TriggerInlineContentAdEvent(placement_id, creative_instance_id,
                                        event_type, std::move(callback));
}

void BatAdsImpl::TriggerSearchResultAdEvent(
    hns_ads::mojom::SearchResultAdInfoPtr ad_mojom,
    const hns_ads::mojom::SearchResultAdEventType event_type,
    TriggerSearchResultAdEventCallback callback) {
  DCHECK(hns_ads::mojom::IsKnownEnumValue(event_type));

  GetAds()->TriggerSearchResultAdEvent(std::move(ad_mojom), event_type,
                                       std::move(callback));
}

void BatAdsImpl::PurgeOrphanedAdEventsForType(
    const hns_ads::mojom::AdType ad_type,
    PurgeOrphanedAdEventsForTypeCallback callback) {
  DCHECK(hns_ads::mojom::IsKnownEnumValue(ad_type));

  GetAds()->PurgeOrphanedAdEventsForType(ad_type, std::move(callback));
}

void BatAdsImpl::GetHistory(const base::Time from_time,
                            const base::Time to_time,
                            GetHistoryCallback callback) {
  const hns_ads::HistoryItemList history_items = GetAds()->GetHistory(
      hns_ads::HistoryFilterType::kConfirmationType,
      hns_ads::HistorySortType::kDescendingOrder, from_time, to_time);

  std::move(callback).Run(hns_ads::HistoryItemsToUIValue(history_items));
}

void BatAdsImpl::GetStatementOfAccounts(
    GetStatementOfAccountsCallback callback) {
  GetAds()->GetStatementOfAccounts(std::move(callback));
}

void BatAdsImpl::GetDiagnostics(GetDiagnosticsCallback callback) {
  GetAds()->GetDiagnostics(std::move(callback));
}

void BatAdsImpl::ToggleLikeAd(base::Value::Dict value,
                              ToggleLikeAdCallback callback) {
  hns_ads::AdContentInfo ad_content = hns_ads::AdContentFromValue(value);
  ad_content.user_reaction_type = GetAds()->ToggleLikeAd(value);
  std::move(callback).Run(AdContentToValue(ad_content));
}

void BatAdsImpl::ToggleDislikeAd(base::Value::Dict value,
                                 ToggleDislikeAdCallback callback) {
  hns_ads::AdContentInfo ad_content = hns_ads::AdContentFromValue(value);
  ad_content.user_reaction_type = GetAds()->ToggleDislikeAd(value);
  std::move(callback).Run(AdContentToValue(ad_content));
}

void BatAdsImpl::ToggleLikeCategory(base::Value::Dict value,
                                    ToggleLikeCategoryCallback callback) {
  hns_ads::CategoryContentInfo category_content =
      hns_ads::CategoryContentFromValue(value);
  category_content.user_reaction_type = GetAds()->ToggleLikeCategory(value);
  std::move(callback).Run(CategoryContentToValue(category_content));
}

void BatAdsImpl::ToggleDislikeCategory(base::Value::Dict value,
                                       ToggleDislikeCategoryCallback callback) {
  hns_ads::CategoryContentInfo category_content =
      hns_ads::CategoryContentFromValue(value);
  category_content.user_reaction_type = GetAds()->ToggleDislikeCategory(value);
  std::move(callback).Run(CategoryContentToValue(category_content));
}

void BatAdsImpl::ToggleSaveAd(base::Value::Dict value,
                              ToggleSaveAdCallback callback) {
  hns_ads::AdContentInfo ad_content = hns_ads::AdContentFromValue(value);
  ad_content.is_saved = GetAds()->ToggleSaveAd(value);
  std::move(callback).Run(AdContentToValue(ad_content));
}

void BatAdsImpl::ToggleMarkAdAsInappropriate(
    base::Value::Dict value,
    ToggleMarkAdAsInappropriateCallback callback) {
  hns_ads::AdContentInfo ad_content = hns_ads::AdContentFromValue(value);
  ad_content.is_flagged = GetAds()->ToggleMarkAdAsInappropriate(value);
  std::move(callback).Run(AdContentToValue(ad_content));
}

hns_ads::Ads* BatAdsImpl::GetAds() {
  DCHECK(ads_instance_);
  DCHECK(ads_instance_->GetAds());
  return ads_instance_->GetAds();
}

}  // namespace bat_ads
