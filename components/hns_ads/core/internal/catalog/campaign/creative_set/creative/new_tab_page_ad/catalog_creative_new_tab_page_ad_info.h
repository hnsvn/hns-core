/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_CREATIVE_NEW_TAB_PAGE_AD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_CREATIVE_NEW_TAB_PAGE_AD_INFO_H_

#include <vector>

#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/catalog_creative_info.h"
#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/new_tab_page_ad/catalog_new_tab_page_ad_payload_info.h"

namespace hns_ads {

struct CatalogCreativeNewTabPageAdInfo final : CatalogCreativeInfo {
  bool operator==(const CatalogCreativeNewTabPageAdInfo&) const;
  bool operator!=(const CatalogCreativeNewTabPageAdInfo&) const;

  CatalogNewTabPageAdPayloadInfo payload;
};

using CatalogCreativeNewTabPageAdList =
    std::vector<CatalogCreativeNewTabPageAdInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_CREATIVE_NEW_TAB_PAGE_AD_INFO_H_
