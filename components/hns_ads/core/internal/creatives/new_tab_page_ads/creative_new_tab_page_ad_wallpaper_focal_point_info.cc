/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_wallpaper_focal_point_info.h"

#include <tuple>

namespace hns_ads {

bool operator==(const CreativeNewTabPageAdWallpaperFocalPointInfo& lhs,
                const CreativeNewTabPageAdWallpaperFocalPointInfo& rhs) {
  const auto tie = [](const CreativeNewTabPageAdWallpaperFocalPointInfo&
                          wallpaper_focal_point) {
    return std::tie(wallpaper_focal_point.x, wallpaper_focal_point.y);
  };

  return tie(lhs) == tie(rhs);
}

bool operator!=(const CreativeNewTabPageAdWallpaperFocalPointInfo& lhs,
                const CreativeNewTabPageAdWallpaperFocalPointInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace hns_ads
