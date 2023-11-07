/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_TIME_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_TIME_UTIL_H_

#include <string>

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

base::Time TimeFromString(const std::string& time_string, bool is_local);

// The distance between the past, present and future is only a persistent
// illusion. Albert Einstein.
base::Time DistantPast();
std::string DistantPastAsISO8601();
base::Time Now();
std::string NowAsISO8601();
base::Time DistantFuture();
std::string DistantFutureAsISO8601();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_TIME_UTIL_H_
