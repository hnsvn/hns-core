/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/allow_notifications_permission_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsAllowNotificationsPermissionRuleTest : public UnitTestBase {
 protected:
  const AllowNotificationsPermissionRule permission_rule_;
};

TEST_F(HnsAdsAllowNotificationsPermissionRuleTest, ShouldAllow) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsAllowNotificationsPermissionRuleTest, ShouldNotAllow) {
  // Arrange
  MockCanShowNotificationAds(ads_client_mock_, false);

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
