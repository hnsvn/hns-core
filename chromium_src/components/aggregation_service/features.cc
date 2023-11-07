/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/components/aggregation_service/features.cc"

#include "base/feature_override.h"

namespace aggregation_service {

OVERRIDE_FEATURE_DEFAULT_STATES({{
    {kAggregationServiceMultipleCloudProviders,
     base::FEATURE_DISABLED_BY_DEFAULT},
}});

}  // namespace aggregation_service
