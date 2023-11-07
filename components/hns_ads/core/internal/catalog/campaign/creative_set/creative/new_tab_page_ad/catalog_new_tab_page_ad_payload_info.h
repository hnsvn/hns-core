/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_NEW_TAB_PAGE_AD_PAYLOAD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_NEW_TAB_PAGE_AD_PAYLOAD_INFO_H_

#include <string>

#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/new_tab_page_ad/catalog_new_tab_page_ad_wallpaper_info.h"
#include "url/gurl.h"

namespace hns_ads {

struct CatalogNewTabPageAdPayloadInfo final {
  CatalogNewTabPageAdPayloadInfo();

  CatalogNewTabPageAdPayloadInfo(const CatalogNewTabPageAdPayloadInfo&);
  CatalogNewTabPageAdPayloadInfo& operator=(
      const CatalogNewTabPageAdPayloadInfo&);

  CatalogNewTabPageAdPayloadInfo(CatalogNewTabPageAdPayloadInfo&&) noexcept;
  CatalogNewTabPageAdPayloadInfo& operator=(
      CatalogNewTabPageAdPayloadInfo&&) noexcept;

  ~CatalogNewTabPageAdPayloadInfo();

  bool operator==(const CatalogNewTabPageAdPayloadInfo&) const;
  bool operator!=(const CatalogNewTabPageAdPayloadInfo&) const;

  std::string company_name;
  GURL image_url;
  std::string alt;
  GURL target_url;
  CatalogNewTabPageAdWallpaperList wallpapers;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_NEW_TAB_PAGE_AD_CATALOG_NEW_TAB_PAGE_AD_PAYLOAD_INFO_H_
