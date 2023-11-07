/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/daypart_exclusion_rule_unittest_util.h"

#include "base/time/time.h"

namespace hns_ads {

int GetMinutesForTesting(const int hours, const int minutes) {
  CHECK(hours >= 0 && hours <= 23);
  CHECK(minutes >= 0 && minutes <= 59);

  const base::TimeDelta time_delta =
      base::Hours(hours) + base::Minutes(minutes);
  return time_delta.InMinutes();
}

}  // namespace hns_ads
