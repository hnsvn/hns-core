/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_HNS_ADS_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_HNS_ADS_FEATURE_H_

#include "base/feature_list.h"

namespace hns_ads {

// Set to |true| to launch as an in process service.
BASE_DECLARE_FEATURE(kShouldLaunchHnsAdsAsAnInProcessServiceFeature);

bool ShouldLaunchAsInProcessService();

// Set to |true| to always run the ads service, even if Hns Private Ads are
// disabled.
BASE_DECLARE_FEATURE(kShouldAlwaysRunHnsAdsServiceFeature);

bool ShouldAlwaysRunService();

// Set to |true| to always trigger new tab page ad events even if Hns Private
// Ads are disabled. |ShouldAlwaysRunService| must be set to |true|, otherwise
// this feature param will be ignored.
BASE_DECLARE_FEATURE(kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature);

bool ShouldAlwaysTriggerNewTabPageAdEvents();

// Set to |true| to support search result ads. |ShouldAlwaysRunService| must be
// set to |true|, otherwise this feature param will be ignored.
BASE_DECLARE_FEATURE(kShouldSupportSearchResultAdsFeature);

bool ShouldSupportSearchResultAds();

// Set to |true| to always trigger search result ad events even if Hns Private
// Ads are disabled. |ShouldAlwaysRunService| must be set to |true|, otherwise
// this feature param will be ignored.
BASE_DECLARE_FEATURE(kShouldAlwaysTriggerHnsSearchResultAdEventsFeature);

bool ShouldAlwaysTriggerSearchResultAdEvents();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FEATURE_HNS_ADS_FEATURE_H_
