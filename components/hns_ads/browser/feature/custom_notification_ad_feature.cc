/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/browser/feature/custom_notification_ad_feature.h"

#include "base/feature_list.h"

namespace hns_ads {

BASE_FEATURE(kCustomNotificationAdFeature,
             "CustomNotificationAds",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool IsCustomNotificationAdFeatureEnabled() {
  return base::FeatureList::IsEnabled(kCustomNotificationAdFeature);
}

}  // namespace hns_ads
