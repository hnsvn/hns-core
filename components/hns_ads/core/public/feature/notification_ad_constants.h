/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_NOTIFICATION_AD_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_NOTIFICATION_AD_CONSTANTS_H_

#include <cstdint>

#include "build/build_config.h"

namespace hns_ads {

// Hns Ads per hour are user configurable within the hns://rewards ads UI
constexpr int64_t kDefaultHnsRewardsNotificationAdsPerHour = 10;

// Default ad notification timeout in seconds.
#if !BUILDFLAG(IS_ANDROID) && !BUILDFLAG(IS_IOS)
constexpr int kDefaultNotificationAdTimeout = 120;
#else
constexpr int kDefaultNotificationAdTimeout = 30;
#endif  // !BUILDFLAG(IS_ANDROID) && !BUILDFLAG(IS_IOS)

// Do not fallback to custom notification ad by default
constexpr bool kDefaultCanFallbackToCustomNotificationAds = false;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_NOTIFICATION_AD_CONSTANTS_H_
