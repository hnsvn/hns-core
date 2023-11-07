/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/new_tab_page_ad_serving_feature.h"

namespace hns_ads {

BASE_FEATURE(kNewTabPageAdServingFeature,
             "NewTabPageAdServing",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool IsNewTabPageAdServingFeatureEnabled() {
  return base::FeatureList::IsEnabled(kNewTabPageAdServingFeature);
}

}  // namespace hns_ads
