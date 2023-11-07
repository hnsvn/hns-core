/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_INLINE_CONTENT_AD_INLINE_CONTENT_AD_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_INLINE_CONTENT_AD_INLINE_CONTENT_AD_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_ads {

BASE_DECLARE_FEATURE(kInlineContentAdFeature);

bool IsInlineContentAdFeatureEnabled();

constexpr base::FeatureParam<int> kMaximumInlineContentAdsPerHour{
    &kInlineContentAdFeature, "maximum_ads_per_hour", 6};

constexpr base::FeatureParam<int> kMaximumInlineContentAdsPerDay{
    &kInlineContentAdFeature, "maximum_ads_per_day", 20};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_INLINE_CONTENT_AD_INLINE_CONTENT_AD_FEATURE_H_
