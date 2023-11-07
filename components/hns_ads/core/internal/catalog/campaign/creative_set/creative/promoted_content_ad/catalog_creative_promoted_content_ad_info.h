/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_PROMOTED_CONTENT_AD_CATALOG_CREATIVE_PROMOTED_CONTENT_AD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_PROMOTED_CONTENT_AD_CATALOG_CREATIVE_PROMOTED_CONTENT_AD_INFO_H_

#include <vector>

#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/catalog_creative_info.h"
#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/promoted_content_ad/catalog_promoted_content_ad_payload_info.h"

namespace hns_ads {

struct CatalogCreativePromotedContentAdInfo final : CatalogCreativeInfo {
  bool operator==(const CatalogCreativePromotedContentAdInfo&) const;
  bool operator!=(const CatalogCreativePromotedContentAdInfo&) const;

  CatalogPromotedContentAdPayloadInfo payload;
};

using CatalogCreativePromotedContentAdList =
    std::vector<CatalogCreativePromotedContentAdInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_PROMOTED_CONTENT_AD_CATALOG_CREATIVE_PROMOTED_CONTENT_AD_INFO_H_
