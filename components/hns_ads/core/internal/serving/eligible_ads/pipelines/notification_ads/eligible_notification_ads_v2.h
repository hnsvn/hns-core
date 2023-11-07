/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NOTIFICATION_ADS_ELIGIBLE_NOTIFICATION_ADS_V2_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NOTIFICATION_ADS_ELIGIBLE_NOTIFICATION_ADS_V2_H_

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_alias.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_base.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace hns_ads {

class AntiTargetingResource;
class SubdivisionTargeting;
struct UserModelInfo;

class EligibleNotificationAdsV2 final : public EligibleNotificationAdsBase {
 public:
  EligibleNotificationAdsV2(
      const SubdivisionTargeting& subdivision_targeting,
      const AntiTargetingResource& anti_targeting_resource);
  ~EligibleNotificationAdsV2() override;

  void GetForUserModel(
      UserModelInfo user_model,
      EligibleAdsCallback<CreativeNotificationAdList> callback) override;

 private:
  void GetForUserModelCallback(
      UserModelInfo user_model,
      EligibleAdsCallback<CreativeNotificationAdList> callback,
      bool success,
      const AdEventList& ad_events);

  void GetBrowsingHistory(
      UserModelInfo user_model,
      const AdEventList& ad_events,
      EligibleAdsCallback<CreativeNotificationAdList> callback);

  void GetEligibleAds(UserModelInfo user_model,
                      const AdEventList& ad_events,
                      EligibleAdsCallback<CreativeNotificationAdList> callback,
                      const BrowsingHistoryList& browsing_history);
  void GetEligibleAdsCallback(
      const UserModelInfo& user_model,
      const AdEventList& ad_events,
      const BrowsingHistoryList& browsing_history,
      EligibleAdsCallback<CreativeNotificationAdList> callback,
      bool success,
      const SegmentList& segments,
      const CreativeNotificationAdList& creative_ads);

  CreativeNotificationAdList FilterCreativeAds(
      const CreativeNotificationAdList& creative_ads,
      const AdEventList& ad_events,
      const BrowsingHistoryList& browsing_history);

  base::WeakPtrFactory<EligibleNotificationAdsV2> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NOTIFICATION_ADS_ELIGIBLE_NOTIFICATION_ADS_V2_H_
