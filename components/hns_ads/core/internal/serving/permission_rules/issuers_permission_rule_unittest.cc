/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/issuers_permission_rule.h"

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsIssuersPermissionRuleTest : public UnitTestBase {
 protected:
  const IssuersPermissionRule permission_rule_;
};

TEST_F(HnsAdsIssuersPermissionRuleTest, ShouldAllowForRewardsUser) {
  // Arrange
  BuildAndSetIssuersForTesting();

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsIssuersPermissionRuleTest, ShouldAlwaysAllowForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsIssuersPermissionRuleTest, ShouldNotAllowIfNoIssuers) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
