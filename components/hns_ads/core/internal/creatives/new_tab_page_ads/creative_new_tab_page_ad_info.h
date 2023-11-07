/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_INFO_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_wallpaper_info.h"
#include "url/gurl.h"

namespace hns_ads {

struct CreativeNewTabPageAdInfo final : CreativeAdInfo {
  CreativeNewTabPageAdInfo();
  explicit CreativeNewTabPageAdInfo(const CreativeAdInfo& creative_ad);

  CreativeNewTabPageAdInfo(const CreativeNewTabPageAdInfo&);
  CreativeNewTabPageAdInfo& operator=(const CreativeNewTabPageAdInfo&);

  CreativeNewTabPageAdInfo(CreativeNewTabPageAdInfo&&) noexcept;
  CreativeNewTabPageAdInfo& operator=(CreativeNewTabPageAdInfo&&) noexcept;

  ~CreativeNewTabPageAdInfo();

  bool operator==(const CreativeNewTabPageAdInfo&) const;
  bool operator!=(const CreativeNewTabPageAdInfo&) const;

  std::string company_name;
  GURL image_url;
  std::string alt;
  CreativeNewTabPageAdWallpaperList wallpapers;
};

using CreativeNewTabPageAdList = std::vector<CreativeNewTabPageAdInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_INFO_H_
