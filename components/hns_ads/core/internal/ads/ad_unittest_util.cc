/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"

#include "base/uuid.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "url/gurl.h"

namespace hns_ads {

AdInfo BuildAdForTesting(const AdType& ad_type,
                         const bool should_use_random_uuids) {
  AdInfo ad;

  ad.type = ad_type;

  ad.placement_id = should_use_random_uuids
                        ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                        : kPlacementId;

  ad.creative_instance_id =
      should_use_random_uuids
          ? base::Uuid::GenerateRandomV4().AsLowercaseString()
          : kCreativeInstanceId;

  ad.creative_set_id = should_use_random_uuids
                           ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                           : kCreativeSetId;

  ad.campaign_id = should_use_random_uuids
                       ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                       : kCampaignId;

  ad.advertiser_id = should_use_random_uuids
                         ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                         : kAdvertiserId;

  ad.segment = kSegment;

  ad.target_url = GURL("https://hns.com");

  return ad;
}

}  // namespace hns_ads
