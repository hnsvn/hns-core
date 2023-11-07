/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/notification_ads/notification_ads_per_hour_permission_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "hns/components/hns_ads/core/public/feature/notification_ad_feature.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNotificationAdsPerHourPermissionRuleTest : public UnitTestBase {
 protected:
  const NotificationAdsPerHourPermissionRule permission_rule_;
};

TEST_F(HnsAdsNotificationAdsPerHourPermissionRuleTest,
       ShouldAllowIfThereAreNoAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsPerHourPermissionRuleTest,
       ShouldAlwaysAllowOnAndroid) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

  const int ads_per_hour = kDefaultNotificationAdsPerHour.Get();

  SetMaximumNotificationAdsPerHourForTesting(ads_per_hour);

  // Act
  RecordAdEventsForTesting(AdType::kNotificationAd, ConfirmationType::kServed,
                           /*count*/ ads_per_hour);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsPerHourPermissionRuleTest,
       ShouldAlwaysAllowOnIOS) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kIOS);

  const int ads_per_hour = kDefaultNotificationAdsPerHour.Get();

  SetMaximumNotificationAdsPerHourForTesting(ads_per_hour);

  // Act
  RecordAdEventsForTesting(AdType::kNotificationAd, ConfirmationType::kServed,
                           /*count*/ ads_per_hour);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsPerHourPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange
  const int ads_per_hour = kDefaultNotificationAdsPerHour.Get();

  SetMaximumNotificationAdsPerHourForTesting(ads_per_hour);

  // Act
  RecordAdEventsForTesting(AdType::kNotificationAd, ConfirmationType::kServed,
                           /*count*/ ads_per_hour - 1);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsPerHourPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCapAfter1Hour) {
  // Arrange
  const int ads_per_hour = kDefaultNotificationAdsPerHour.Get();

  SetMaximumNotificationAdsPerHourForTesting(ads_per_hour);

  RecordAdEventsForTesting(AdType::kNotificationAd, ConfirmationType::kServed,
                           /*count*/ ads_per_hour);

  // Act
  AdvanceClockBy(base::Hours(1));

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsPerHourPermissionRuleTest,
       ShouldNotAllowIfExceedsCapWithin1Hour) {
  // Arrange
  const int ads_per_hour = kDefaultNotificationAdsPerHour.Get();

  SetMaximumNotificationAdsPerHourForTesting(ads_per_hour);

  RecordAdEventsForTesting(AdType::kNotificationAd, ConfirmationType::kServed,
                           /*count*/ ads_per_hour);

  // Act
  AdvanceClockBy(base::Hours(1) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
