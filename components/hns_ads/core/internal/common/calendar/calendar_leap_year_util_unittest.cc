/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/calendar/calendar_leap_year_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCalendarLeapYearUtilTest, IsLeapYear) {
  // Arrange

  // Act

  // Assert
  for (int year = 2000; year < 2050; year++) {
    EXPECT_EQ(year % 4 == 0, IsLeapYear(year));
  }
}

}  // namespace hns_ads
