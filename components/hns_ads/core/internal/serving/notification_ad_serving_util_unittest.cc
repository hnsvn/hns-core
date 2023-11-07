/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/notification_ad_serving_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNotificationAdServingUtilTest : public UnitTestBase {};

TEST_F(HnsAdsNotificationAdServingUtilTest,
       ShouldServeAdsAtRegularIntervalsOnIOS) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kIOS);

  // Act

  // Assert
  EXPECT_TRUE(ShouldServeAdsAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       ShouldServeAdsAtRegularIntervalsOnAndroid) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

  // Act

  // Assert
  EXPECT_TRUE(ShouldServeAdsAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       ShouldNotServeAdsAtRegularIntervalsOnMacOS) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kMacOS);

  // Act

  // Assert
  EXPECT_FALSE(ShouldServeAdsAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       ShouldNotServeAdsAtRegularIntervalsOnWindows) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kWindows);

  // Act

  // Assert
  EXPECT_FALSE(ShouldServeAdsAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       ShouldNotServeAdsAtRegularIntervalsOnLinux) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kLinux);

  // Act

  // Assert
  EXPECT_FALSE(ShouldServeAdsAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdServingUtilTest, SetServeAdAt) {
  // Arrange

  // Act
  SetServeAdAt(DistantFuture());

  // Assert
  EXPECT_EQ(DistantFuture(), ServeAdAt());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       CalculateDelayBeforeServingTheFirstAd) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(2), CalculateDelayBeforeServingAnAd());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       CalculateDelayBeforeServingAPastDueAd) {
  // Arrange
  SetServeAdAt(DistantPast());

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(1), CalculateDelayBeforeServingAnAd());
}

TEST_F(HnsAdsNotificationAdServingUtilTest, CalculateDelayBeforeServingAnAd) {
  // Arrange
  SetServeAdAt(DistantFuture());

  // Act

  // Assert
  EXPECT_EQ(DistantFuture() - Now(), CalculateDelayBeforeServingAnAd());
}

TEST_F(HnsAdsNotificationAdServingUtilTest,
       CalculateMinimumDelayBeforeServingAnAd) {
  // Arrange
  SetServeAdAt(Now());

  AdvanceClockBy(base::Milliseconds(1));

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(1), CalculateDelayBeforeServingAnAd());
}

}  // namespace hns_ads
