/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CALENDAR_CALENDAR_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CALENDAR_CALENDAR_UTIL_H_

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

// Four digit year "2007", 1-based month (values 1 = January, etc.)
int GetLastDayOfMonth(int year, int month);

// Four digit year "2007", 1-based month (values 1 = January, etc.), 1-based day
// of month (1-31)
int GetDayOfWeek(int year, int month, int day);

int GetDayOfWeek(base::Time time, bool is_local);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CALENDAR_CALENDAR_UTIL_H_
