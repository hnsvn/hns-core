/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/calendar/calendar_leap_year_util.h"

namespace hns_ads {

bool IsLeapYear(const int year) {
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

}  // namespace hns_ads
