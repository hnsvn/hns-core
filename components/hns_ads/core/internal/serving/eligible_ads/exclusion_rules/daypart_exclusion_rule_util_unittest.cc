/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/daypart_exclusion_rule_util.h"

#include "hns/components/hns_ads/core/internal/creatives/creative_daypart_info.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/daypart_exclusion_rule_unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsDaypartExclusionRuleUtilTest, MatchDayOfWeek) {
  // Arrange
  CreativeDaypartInfo daypart;
  daypart.days_of_week = "0123456";

  // Act

  // Assert
  EXPECT_TRUE(MatchDayOfWeek(daypart, '3'));
}

TEST(HnsAdsDaypartExclusionRuleUtilTest, DoNotMatchDayOfWeek) {
  // Arrange
  CreativeDaypartInfo daypart;
  daypart.days_of_week = "012456";

  // Act

  // Assert
  EXPECT_FALSE(MatchDayOfWeek(daypart, '3'));
}

TEST(HnsAdsDaypartExclusionRuleUtilTest, MatchTimeSlot) {
  // Arrange
  CreativeDaypartInfo daypart;
  daypart.start_minute = GetMinutesForTesting(/*hours*/ 0, /*minutes*/ 0);
  daypart.end_minute = GetMinutesForTesting(/*hours*/ 23, /*minutes*/ 59);

  // Act

  // Assert
  EXPECT_TRUE(MatchTimeSlot(daypart,
                            GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 35)));
}

TEST(HnsAdsDaypartExclusionRuleUtilTest, MatchExactTimeSlot) {
  // Arrange
  CreativeDaypartInfo daypart;
  daypart.start_minute = GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 35);
  daypart.end_minute = GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 35);

  // Act

  // Assert
  EXPECT_TRUE(MatchTimeSlot(daypart,
                            GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 35)));
}

TEST(HnsAdsDaypartExclusionRuleUtilTest, DoNotMatchTimeSlotAfterEndMinute) {
  // Arrange
  CreativeDaypartInfo daypart;
  daypart.start_minute = GetMinutesForTesting(/*hours*/ 0, /*minutes*/ 0);
  daypart.end_minute = GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 34);

  // Act

  // Assert
  EXPECT_FALSE(MatchTimeSlot(
      daypart, GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 35)));
}

TEST(HnsAdsDaypartExclusionRuleUtilTest,
     DoNotMatchTimeSlotBeforeStartMinute) {
  // Arrange
  CreativeDaypartInfo daypart;
  daypart.start_minute = GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 36);
  daypart.end_minute = GetMinutesForTesting(/*hours*/ 23, /*minutes*/ 59);

  // Act

  // Assert
  EXPECT_FALSE(MatchTimeSlot(
      daypart, GetMinutesForTesting(/*hours*/ 5, /*minutes*/ 35)));
}

}  // namespace hns_ads
