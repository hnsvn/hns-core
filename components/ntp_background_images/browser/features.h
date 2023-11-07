/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_FEATURES_H_
#define HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_FEATURES_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace ntp_background_images {
namespace features {

BASE_DECLARE_FEATURE(kHnsNTPBrandedWallpaperDemo);

BASE_DECLARE_FEATURE(kHnsNTPSuperReferralWallpaper);

BASE_DECLARE_FEATURE(kHnsNTPBrandedWallpaper);

// Show initial branded wallpaper after nth new tab page for fresh installs.
constexpr base::FeatureParam<int> kInitialCountToBrandedWallpaper{
    &kHnsNTPBrandedWallpaper, "initial_count_to_branded_wallpaper", 1};

// Show branded wallpaper every nth new tab page.
constexpr base::FeatureParam<int> kCountToBrandedWallpaper{
    &kHnsNTPBrandedWallpaper, "count_to_branded_wallpaper", 3};

}  // namespace features
}  // namespace ntp_background_images

#endif  // HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_FEATURES_H_
