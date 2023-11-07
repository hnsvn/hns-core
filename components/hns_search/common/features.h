// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_COMMON_FEATURES_H_
#define HNS_COMPONENTS_HNS_SEARCH_COMMON_FEATURES_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_search {
namespace features {

constexpr char kHnsSearchDefaultAPIDailyLimitName[] =
    "HnsSearchDefaultAPIDailyLimit";
constexpr char kHnsSearchDefaultAPITotalLimitName[] =
    "HnsSearchDefaultAPITotalLimit";

BASE_DECLARE_FEATURE(kHnsSearchDefaultAPIFeature);
extern const base::FeatureParam<int> kHnsSearchDefaultAPIDailyLimit;
extern const base::FeatureParam<int> kHnsSearchDefaultAPITotalLimit;

}  // namespace features
}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_COMMON_FEATURES_H_
