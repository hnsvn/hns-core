/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsUserIdleDetectionTest : public UnitTestBase {};

TEST_F(HnsAdsUserIdleDetectionTest, RewardsUserDidBecomeActive) {
  // Arrange
  const UserIdleDetection user_idle_detection;

  // Assert
  EXPECT_CALL(ads_client_mock_, Log);

  // Act
  NotifyUserDidBecomeActive(/*idle_time*/ base::Seconds(10),
                            /*screen_was_locked*/ false);
}

TEST_F(HnsAdsUserIdleDetectionTest, NonRewardsUserDidBecomeActive) {
  // Arrange
  DisableHnsRewardsForTesting();

  const UserIdleDetection user_idle_detection;

  // Assert
  EXPECT_CALL(ads_client_mock_, Log).Times(0);

  // Act
  NotifyUserDidBecomeActive(/*idle_time*/ base::Seconds(10),
                            /*screen_was_locked*/ false);
}

TEST_F(HnsAdsUserIdleDetectionTest,
       RewardsUserDidBecomeActiveWhileScreenWasLocked) {
  // Arrange
  const UserIdleDetection user_idle_detection;

  // Assert
  EXPECT_CALL(ads_client_mock_, Log).Times(2);

  // Act
  NotifyUserDidBecomeActive(/*idle_time*/ base::Seconds(10),
                            /*screen_was_locked*/ true);
}

TEST_F(HnsAdsUserIdleDetectionTest,
       NonRewardsUserDidBecomeActiveWhileScreenWasLocked) {
  // Arrange
  DisableHnsRewardsForTesting();

  const UserIdleDetection user_idle_detection;

  // Assert
  EXPECT_CALL(ads_client_mock_, Log).Times(0);

  // Act
  NotifyUserDidBecomeActive(/*idle_time*/ base::Seconds(10),
                            /*screen_was_locked*/ true);
}

TEST_F(HnsAdsUserIdleDetectionTest, RewardsUserDidBecomeIdle) {
  // Arrange
  const UserIdleDetection user_idle_detection;

  // Assert
  EXPECT_CALL(ads_client_mock_, Log);

  // Act
  NotifyUserDidBecomeIdle();
}

TEST_F(HnsAdsUserIdleDetectionTest, NonRewardsUserDidBecomeIdle) {
  // Arrange
  DisableHnsRewardsForTesting();

  const UserIdleDetection user_idle_detection;

  // Assert
  EXPECT_CALL(ads_client_mock_, Log).Times(0);

  // Act
  NotifyUserDidBecomeIdle();
}

}  // namespace hns_ads
