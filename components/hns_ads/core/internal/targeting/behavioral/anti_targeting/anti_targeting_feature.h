/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_ANTI_TARGETING_ANTI_TARGETING_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_ANTI_TARGETING_ANTI_TARGETING_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_ads {

BASE_DECLARE_FEATURE(kAntiTargetingFeature);

bool IsAntiTargetingFeatureEnabled();

constexpr base::FeatureParam<int> kAntiTargetingResourceVersion{
    &kAntiTargetingFeature, "resource_version", 1};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_ANTI_TARGETING_ANTI_TARGETING_FEATURE_H_
