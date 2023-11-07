/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_AD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_AD_INFO_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"

namespace hns_ads {

struct CreativePromotedContentAdInfo final : CreativeAdInfo {
  CreativePromotedContentAdInfo();
  explicit CreativePromotedContentAdInfo(const CreativeAdInfo& creative_ad);

  bool operator==(const CreativePromotedContentAdInfo&) const;
  bool operator!=(const CreativePromotedContentAdInfo&) const;

  std::string title;
  std::string description;
};

using CreativePromotedContentAdList =
    std::vector<CreativePromotedContentAdInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_PROMOTED_CONTENT_ADS_CREATIVE_PROMOTED_CONTENT_AD_INFO_H_
