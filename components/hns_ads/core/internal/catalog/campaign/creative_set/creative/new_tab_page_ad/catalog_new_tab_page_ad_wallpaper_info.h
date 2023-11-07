/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_NEW_TAB_PAGE_AD_WALLPAPER_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_NEW_TAB_PAGE_AD_WALLPAPER_INFO_H_

#include <vector>

#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/new_tab_page_ad/catalog_new_tab_page_ad_wallpaper_focal_point_info.h"
#include "url/gurl.h"

namespace hns_ads {

struct CatalogNewTabPageAdWallpaperInfo final {
  bool operator==(const CatalogNewTabPageAdWallpaperInfo&) const;
  bool operator!=(const CatalogNewTabPageAdWallpaperInfo&) const;

  GURL image_url;
  CatalogNewTabPageAdWallpaperFocalPointInfo focal_point;
};

using CatalogNewTabPageAdWallpaperList =
    std::vector<CatalogNewTabPageAdWallpaperInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_NEW_TAB_PAGE_AD_WALLPAPER_INFO_H_
