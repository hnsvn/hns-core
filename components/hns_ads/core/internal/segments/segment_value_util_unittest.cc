/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/segments/segment_value_util.h"

#include "base/test/values_test_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kSegmentsAsJson[] =
    R"(["technology & computing","personal finance-banking","food & drink-restaurants"])";
constexpr char kNoSegmentsAsJson[] = "[]";

}  // namespace

TEST(HnsAdsSegmentValueUtilTest, SegmentsToValue) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(
      base::test::ParseJsonList(kSegmentsAsJson),
      SegmentsToValue({"technology & computing", "personal finance-banking",
                       "food & drink-restaurants"}));
}

TEST(HnsAdsSegmentValueUtilTest, NoSegmentsToValue) {
  // Arrange

  // Act
  const base::Value::List list = SegmentsToValue({});

  // Assert
  EXPECT_TRUE(list.empty());
}

TEST(HnsAdsSegmentValueUtilTest, SegmentsFromValue) {
  // Arrange
  const base::Value::List list = base::test::ParseJsonList(kSegmentsAsJson);

  // Act

  // Assert
  const SegmentList expected_segments = {"technology & computing",
                                         "personal finance-banking",
                                         "food & drink-restaurants"};
  EXPECT_EQ(expected_segments, SegmentsFromValue(list));
}

TEST(HnsAdsSegmentValueUtilTest, NoSegmentsFromValue) {
  // Arrange
  const base::Value::List list = base::test::ParseJsonList(kNoSegmentsAsJson);

  // Act
  const SegmentList segments = SegmentsFromValue(list);

  // Assert
  EXPECT_TRUE(segments.empty());
}

}  // namespace hns_ads
