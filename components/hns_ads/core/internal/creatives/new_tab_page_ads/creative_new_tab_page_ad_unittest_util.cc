/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_unittest_util.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_wallpaper_focal_point_info.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_wallpaper_info.h"
#include "url/gurl.h"

namespace hns_ads {

CreativeNewTabPageAdList BuildCreativeNewTabPageAdsForTesting(const int count) {
  CHECK_GT(count, 0);

  CreativeNewTabPageAdList creative_ads;

  for (int i = 0; i < count; i++) {
    const CreativeNewTabPageAdInfo creative_ad =
        BuildCreativeNewTabPageAdForTesting(/*should_use_random_uuids*/ true);
    creative_ads.push_back(creative_ad);
  }

  return creative_ads;
}

CreativeNewTabPageAdInfo BuildCreativeNewTabPageAdForTesting(
    const bool should_use_random_uuids) {
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(should_use_random_uuids);
  CreativeNewTabPageAdInfo creative_new_tab_page_ad(creative_ad);

  creative_new_tab_page_ad.company_name = "Test Ad Company Name";
  creative_new_tab_page_ad.image_url = GURL("https://hns.com/image");
  creative_new_tab_page_ad.alt = "Test Ad Alt";

  CreativeNewTabPageAdWallpaperInfo wallpaper;
  wallpaper.image_url = GURL("https://hns.com/wallpaper_image");
  CreativeNewTabPageAdWallpaperFocalPointInfo wallpaper_focal_point;
  wallpaper_focal_point.x = 1280;
  wallpaper_focal_point.y = 720;
  wallpaper.focal_point = wallpaper_focal_point;
  creative_new_tab_page_ad.wallpapers.push_back(wallpaper);

  return creative_new_tab_page_ad;
}

}  // namespace hns_ads
