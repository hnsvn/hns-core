/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/analytics/p2a/p2a_value_util.h"

#include "base/test/values_test_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::p2a {

namespace {
constexpr char kEventsAsJson[] = R"(["event_1","event_2"])";
}  // namespace

TEST(HnsAdsP2AValueUtilTest, EventsToValue) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonList(kEventsAsJson),
            EventsToValue({"event_1", "event_2"}));
}

TEST(HnsAdsP2AValueUtilTest, EmptyEventsToValue) {
  // Arrange

  // Act
  const base::Value::List events = EventsToValue({});

  // Assert
  EXPECT_TRUE(events.empty());
}

}  // namespace hns_ads::p2a
