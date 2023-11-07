/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/calendar/calendar_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCalendarUtilTest, GetLastDayOfMonth) {
  // Arrange
  constexpr int kLastDayForMonth[12] = {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

  // Act

  // Assert
  for (int i = 0; i < 12; i++) {
    EXPECT_EQ(kLastDayForMonth[i],
              GetLastDayOfMonth(/*year*/ 2021, /*month*/ i + 1));
  }
}

TEST(HnsAdsCalendarUtilTest, GetLastDayOfMonthForLeapYear) {
  // Arrange
  constexpr int kLastDayForMonth[12] = {31, 29, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

  // Act

  // Assert
  for (int i = 0; i < 12; i++) {
    EXPECT_EQ(kLastDayForMonth[i],
              GetLastDayOfMonth(/*year*/ 2020, /*month*/ i + 1));
  }
}

TEST(HnsAdsCalendarUtilTest, GetDayOfWeekForYearMonthAndDay) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(6, GetDayOfWeek(/*year*/ 2020, /*month*/ 2, /*day*/ 29));
}

TEST(HnsAdsCalendarUtilTest, GetDayOfWeek) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(3,
            GetDayOfWeek(TimeFromString("November 18 1970", /*is_local*/ false),
                         /*is_local*/ false));
}

}  // namespace hns_ads
