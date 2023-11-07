/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/feature/notification_ad_feature.h"

namespace hns_ads {

BASE_FEATURE(kNotificationAdFeature,
             "NotificationAds",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kAllowedToFallbackToCustomNotificationAdFeature,
             "AllowedToFallbackToCustomNotificationAd",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool IsNotificationAdFeatureEnabled() {
  return base::FeatureList::IsEnabled(kNotificationAdFeature);
}

bool IsAllowedToFallbackToCustomNotificationAdFeatureEnabled() {
  return base::FeatureList::IsEnabled(
      kAllowedToFallbackToCustomNotificationAdFeature);
}

}  // namespace hns_ads
