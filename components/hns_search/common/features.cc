// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/components/hns_search/common/features.h"

#include "base/feature_list.h"

namespace hns_search {
namespace features {

BASE_FEATURE(kHnsSearchDefaultAPIFeature,
             "HnsSearchDefaultAPI",
             base::FEATURE_ENABLED_BY_DEFAULT);

const base::FeatureParam<int> kHnsSearchDefaultAPIDailyLimit{
    &kHnsSearchDefaultAPIFeature, kHnsSearchDefaultAPIDailyLimitName, 3};

const base::FeatureParam<int> kHnsSearchDefaultAPITotalLimit{
    &kHnsSearchDefaultAPIFeature, kHnsSearchDefaultAPITotalLimitName, 10};

}  // namespace features
}  // namespace hns_search
