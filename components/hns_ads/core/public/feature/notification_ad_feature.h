/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_NOTIFICATION_AD_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_NOTIFICATION_AD_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"
#include "hns/components/hns_ads/core/public/feature/notification_ad_constants.h"

namespace hns_ads {

BASE_DECLARE_FEATURE(kNotificationAdFeature);
BASE_DECLARE_FEATURE(kAllowedToFallbackToCustomNotificationAdFeature);

bool IsNotificationAdFeatureEnabled();

bool IsAllowedToFallbackToCustomNotificationAdFeatureEnabled();

// Ad notification timeout in seconds. Set to 0 to never time out
constexpr base::FeatureParam<int> kNotificationAdTimeout{
    &kNotificationAdFeature, "notification_ad_timeout",
    kDefaultNotificationAdTimeout};

constexpr base::FeatureParam<int> kDefaultNotificationAdsPerHour{
    &kNotificationAdFeature, "default_ads_per_hour",
    kDefaultHnsRewardsNotificationAdsPerHour};

constexpr base::FeatureParam<int> kMaximumNotificationAdsPerDay{
    &kNotificationAdFeature, "maximum_ads_per_day", 100};

// Set to true to fallback to custom notification ads if native notifications
// are disabled or false to never fallback
constexpr base::FeatureParam<bool> kCanFallbackToCustomNotificationAds{
    &kNotificationAdFeature, "can_fallback_to_custom_notifications",
    kDefaultCanFallbackToCustomNotificationAds};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_NOTIFICATION_AD_FEATURE_H_
