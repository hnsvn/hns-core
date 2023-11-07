/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_TIME_TIME_CONSTRAINT_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_TIME_TIME_CONSTRAINT_UTIL_H_

#include <vector>

namespace base {
class Time;
class TimeDelta;
}  // namespace base

namespace hns_ads {

bool DoesHistoryRespectRollingTimeConstraint(
    const std::vector<base::Time>& history,
    base::TimeDelta time_constraint,
    size_t cap);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_TIME_TIME_CONSTRAINT_UTIL_H_