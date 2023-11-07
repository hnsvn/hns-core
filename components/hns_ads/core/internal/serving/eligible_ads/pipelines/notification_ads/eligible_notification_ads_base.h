/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NOTIFICATION_ADS_ELIGIBLE_NOTIFICATION_ADS_BASE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NOTIFICATION_ADS_ELIGIBLE_NOTIFICATION_ADS_BASE_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/eligible_ads_callback.h"
#include "hns/components/hns_ads/core/public/ad_info.h"

namespace hns_ads {

class AntiTargetingResource;
class SubdivisionTargeting;
struct UserModelInfo;

class EligibleNotificationAdsBase {
 public:
  EligibleNotificationAdsBase(const EligibleNotificationAdsBase&) = delete;
  EligibleNotificationAdsBase& operator=(const EligibleNotificationAdsBase&) =
      delete;

  EligibleNotificationAdsBase(EligibleNotificationAdsBase&&) noexcept = delete;
  EligibleNotificationAdsBase& operator=(
      EligibleNotificationAdsBase&&) noexcept = delete;

  virtual ~EligibleNotificationAdsBase();

  virtual void GetForUserModel(
      UserModelInfo user_model,
      EligibleAdsCallback<CreativeNotificationAdList> callback) = 0;

  void SetLastServedAd(const AdInfo& ad) { last_served_ad_ = ad; }

 protected:
  EligibleNotificationAdsBase(
      const SubdivisionTargeting& subdivision_targeting,
      const AntiTargetingResource& anti_targeting_resource);

  const raw_ref<const SubdivisionTargeting> subdivision_targeting_;
  const raw_ref<const AntiTargetingResource> anti_targeting_resource_;

  AdInfo last_served_ad_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NOTIFICATION_ADS_ELIGIBLE_NOTIFICATION_ADS_BASE_H_
