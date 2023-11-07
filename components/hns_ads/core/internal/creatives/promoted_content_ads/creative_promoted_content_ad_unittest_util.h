/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_AD_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_AD_UNITTEST_UTIL_H_

#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ad_info.h"

namespace hns_ads {

CreativePromotedContentAdList BuildCreativePromotedContentAdsForTesting(
    int count);
CreativePromotedContentAdInfo BuildCreativePromotedContentAdForTesting(
    bool should_use_random_uuids);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_AD_UNITTEST_UTIL_H_
