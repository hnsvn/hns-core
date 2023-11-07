/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_WALLPAPER_FOCAL_POINT_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_WALLPAPER_FOCAL_POINT_INFO_H_

namespace hns_ads {

struct CreativeNewTabPageAdWallpaperFocalPointInfo final {
  int x = 0;
  int y = 0;
};

bool operator==(const CreativeNewTabPageAdWallpaperFocalPointInfo&,
                const CreativeNewTabPageAdWallpaperFocalPointInfo&);
bool operator!=(const CreativeNewTabPageAdWallpaperFocalPointInfo&,
                const CreativeNewTabPageAdWallpaperFocalPointInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_WALLPAPER_FOCAL_POINT_INFO_H_
