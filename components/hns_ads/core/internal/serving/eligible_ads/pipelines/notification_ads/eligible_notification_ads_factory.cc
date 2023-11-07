/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_factory.h"

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_base.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_v1.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_v2.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_v3.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/anti_targeting/resource/anti_targeting_resource.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"

namespace hns_ads {

std::unique_ptr<EligibleNotificationAdsBase>
EligibleNotificationAdsFactory::Build(
    const int version,
    const SubdivisionTargeting& subdivision_targeting,
    const AntiTargetingResource& anti_targeting_resource) {
  switch (version) {
    case 1: {
      return std::make_unique<EligibleNotificationAdsV1>(
          subdivision_targeting, anti_targeting_resource);
    }

    case 2: {
      return std::make_unique<EligibleNotificationAdsV2>(
          subdivision_targeting, anti_targeting_resource);
    }

    case 3: {
      return std::make_unique<EligibleNotificationAdsV3>(
          subdivision_targeting, anti_targeting_resource);
    }

    default: {
      return nullptr;
    }
  }
}

}  // namespace hns_ads