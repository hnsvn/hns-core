/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/confirmation_tokens_permission_rule.h"

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConfirmationTokensPermissionRuleTest : public UnitTestBase {
 protected:
  const ConfirmationTokensPermissionRule permission_rule_;
};

TEST_F(HnsAdsConfirmationTokensPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange
  SetConfirmationTokensForTesting(/*count*/ 10);

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsConfirmationTokensPermissionRuleTest,
       ShouldAllowIfUserHasNotJoinedHnsRewards) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsConfirmationTokensPermissionRuleTest,
       ShouldNotAllowIfNoConfirmationTokens) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsConfirmationTokensPermissionRuleTest,
       ShouldNotAllowIfExceedsCap) {
  // Arrange
  SetConfirmationTokensForTesting(/*count*/ 9);

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
