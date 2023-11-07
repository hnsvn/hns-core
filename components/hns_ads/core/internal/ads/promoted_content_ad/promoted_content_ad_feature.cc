/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ads/promoted_content_ad/promoted_content_ad_feature.h"

namespace hns_ads {

BASE_FEATURE(kPromotedContentAdFeature,
             "PromotedContentAds",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool IsPromotedContentAdFeatureEnabled() {
  return base::FeatureList::IsEnabled(kPromotedContentAdFeature);
}

}  // namespace hns_ads
