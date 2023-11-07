/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/feature/hns_ads_feature.h"

namespace hns_ads {

BASE_FEATURE(kShouldLaunchHnsAdsAsAnInProcessServiceFeature,
             "ShouldLaunchHnsAdsAsInProcessService",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool ShouldLaunchAsInProcessService() {
  return base::FeatureList::IsEnabled(
      kShouldLaunchHnsAdsAsAnInProcessServiceFeature);
}

BASE_FEATURE(kShouldAlwaysRunHnsAdsServiceFeature,
             "ShouldAlwaysRunHnsAdsService",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool ShouldAlwaysRunService() {
  return base::FeatureList::IsEnabled(kShouldAlwaysRunHnsAdsServiceFeature);
}

BASE_FEATURE(kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature,
             "ShouldAlwaysTriggerHnsNewTabPageAdEvents",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool ShouldAlwaysTriggerNewTabPageAdEvents() {
  return base::FeatureList::IsEnabled(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature);
}

BASE_FEATURE(kShouldSupportSearchResultAdsFeature,
             "ShouldSupportSearchResultAds",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool ShouldSupportSearchResultAds() {
  return base::FeatureList::IsEnabled(kShouldSupportSearchResultAdsFeature);
}

BASE_FEATURE(kShouldAlwaysTriggerHnsSearchResultAdEventsFeature,
             "ShouldAlwaysTriggerHnsSearchResultAdEvents",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool ShouldAlwaysTriggerSearchResultAdEvents() {
  return base::FeatureList::IsEnabled(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature);
}

}  // namespace hns_ads
