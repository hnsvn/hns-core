/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ads/new_tab_page_ad/new_tab_page_ad_feature.h"

namespace hns_ads {

BASE_FEATURE(kNewTabPageAdFeature,
             "NewTabPageAds",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool IsNewTabPageAdFeatureEnabled() {
  return base::FeatureList::IsEnabled(kNewTabPageAdFeature);
}

}  // namespace hns_ads
