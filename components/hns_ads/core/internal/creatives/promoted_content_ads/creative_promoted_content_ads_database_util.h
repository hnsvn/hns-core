/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_ADS_DATABASE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_ADS_DATABASE_UTIL_H_

#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ad_info.h"

namespace hns_ads::database {

void DeleteCreativePromotedContentAds();

void SaveCreativePromotedContentAds(
    const CreativePromotedContentAdList& creative_ads);

}  // namespace hns_ads::database

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_ADS_DATABASE_UTIL_H_
