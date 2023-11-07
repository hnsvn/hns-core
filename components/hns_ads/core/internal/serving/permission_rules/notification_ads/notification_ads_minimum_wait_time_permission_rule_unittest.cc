/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/notification_ads/notification_ads_minimum_wait_time_permission_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNotificationAdsMinimumWaitTimePermissionRuleTest
    : public UnitTestBase {
 protected:
  const NotificationAdMinimumWaitTimePermissionRule permission_rule_;
};

TEST_F(HnsAdsNotificationAdsMinimumWaitTimePermissionRuleTest,
       ShouldAllowIfThereAreNoAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsMinimumWaitTimePermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange
  SetMaximumNotificationAdsPerHourForTesting(5);

  RecordAdEventForTesting(AdType::kNotificationAd, ConfirmationType::kServed);

  // Act
  AdvanceClockBy(base::Minutes(12));

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNotificationAdsMinimumWaitTimePermissionRuleTest,
       ShouldNotAllowIfExceedsCap) {
  // Arrange
  SetMaximumNotificationAdsPerHourForTesting(5);

  RecordAdEventForTesting(AdType::kNotificationAd, ConfirmationType::kServed);

  // Act
  AdvanceClockBy(base::Minutes(12) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
