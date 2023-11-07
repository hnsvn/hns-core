/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/do_not_disturb_permission_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsDoNotDisturbPermissionRuleTest : public UnitTestBase {};

TEST_F(HnsAdsDoNotDisturbPermissionRuleTest,
       ShouldAllowWhileBrowserIsInactiveBetween6amAnd9pmOnAndroid) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

  NotifyBrowserDidResignActive();
  NotifyBrowserDidEnterBackground();

  AdvanceClockToMidnight(/*is_local*/ true);

  // Act
  {
    // Verify 5:59 AM
    AdvanceClockBy(base::Hours(5) + base::Minutes(59));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_FALSE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 6:00 AM
    AdvanceClockBy(base::Minutes(1));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 8:59 PM
    AdvanceClockBy(base::Hours(14) + base::Minutes(59));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 9:00 PM
    AdvanceClockBy(base::Minutes(1));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_FALSE(permission_rule.ShouldAllow().has_value());
  }
}

TEST_F(HnsAdsDoNotDisturbPermissionRuleTest,
       ShouldAllowWhileBrowserIsActiveOnAndroid) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

  NotifyBrowserDidBecomeActive();
  NotifyBrowserDidEnterForeground();

  AdvanceClockToMidnight(/*is_local*/ true);

  // Act
  {
    // Verify 5:59 AM
    AdvanceClockBy(base::Hours(5) + base::Minutes(59));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 6:00 AM
    AdvanceClockBy(base::Minutes(1));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 8:59 PM
    AdvanceClockBy(base::Hours(14) + base::Minutes(59));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 9:00 PM
    AdvanceClockBy(base::Minutes(1));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }
}

TEST_F(HnsAdsDoNotDisturbPermissionRuleTest, ShouldAlwaysAllowOnIOS) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kIOS);

  NotifyBrowserDidBecomeActive();
  NotifyBrowserDidEnterForeground();

  AdvanceClockToMidnight(/*is_local*/ true);

  // Act
  {
    // Verify 00:00 AM

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 12:00 PM
    AdvanceClockBy(base::Hours(12));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }
}

TEST_F(HnsAdsDoNotDisturbPermissionRuleTest, ShouldAlwaysAllowOnMacOS) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kMacOS);

  NotifyBrowserDidBecomeActive();
  NotifyBrowserDidEnterForeground();

  AdvanceClockToMidnight(/*is_local*/ true);

  // Act
  {
    // Verify 00:00 AM

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 12:00 PM
    AdvanceClockBy(base::Hours(12));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }
}

TEST_F(HnsAdsDoNotDisturbPermissionRuleTest, ShouldAlwaysAllowOnWindows) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kWindows);

  NotifyBrowserDidBecomeActive();
  NotifyBrowserDidEnterForeground();

  AdvanceClockToMidnight(/*is_local*/ true);

  // Act
  {
    // Verify 00:00 AM

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 12:00 PM
    AdvanceClockBy(base::Hours(12));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }
}

TEST_F(HnsAdsDoNotDisturbPermissionRuleTest, ShouldAlwaysAllowOnLinux) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kLinux);

  NotifyBrowserDidBecomeActive();
  NotifyBrowserDidEnterForeground();

  AdvanceClockToMidnight(/*is_local*/ true);

  // Act
  {
    // Verify 00:00 AM

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }

  {
    // Verify 12:00 PM
    AdvanceClockBy(base::Hours(12));

    // Assert
    const DoNotDisturbPermissionRule permission_rule;
    EXPECT_TRUE(permission_rule.ShouldAllow().has_value());
  }
}

}  // namespace hns_ads
