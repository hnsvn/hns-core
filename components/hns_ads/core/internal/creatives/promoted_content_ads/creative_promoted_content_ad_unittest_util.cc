/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ad_unittest_util.h"

#include "base/check_op.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ad_info.h"

namespace hns_ads {

CreativePromotedContentAdList BuildCreativePromotedContentAdsForTesting(
    const int count) {
  CHECK_GT(count, 0);

  CreativePromotedContentAdList creative_ads;

  for (int i = 0; i < count; i++) {
    const CreativePromotedContentAdInfo creative_ad =
        BuildCreativePromotedContentAdForTesting(
            /*should_use_random_uuids*/ true);
    creative_ads.push_back(creative_ad);
  }

  return creative_ads;
}

CreativePromotedContentAdInfo BuildCreativePromotedContentAdForTesting(
    const bool should_use_random_uuids) {
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(should_use_random_uuids);
  CreativePromotedContentAdInfo creative_promoted_content_ad(creative_ad);

  creative_promoted_content_ad.title = "Test Ad Title";
  creative_promoted_content_ad.description = "Test Ad Description";

  return creative_promoted_content_ad;
}

}  // namespace hns_ads
