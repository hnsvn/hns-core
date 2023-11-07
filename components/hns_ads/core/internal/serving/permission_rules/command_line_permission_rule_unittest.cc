/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/command_line_permission_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCommandLinePermissionRuleTest : public UnitTestBase {
 protected:
  const CommandLinePermissionRule permission_rule_;
};

TEST_F(HnsAdsCommandLinePermissionRuleTest,
       ShouldAllowIfDidNotOverrideCommandLineSwitchesForProduction) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kProduction;

  GlobalState::GetInstance()->Flags().did_override_from_command_line = false;

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsCommandLinePermissionRuleTest,
       ShouldAllowIfDidNotOverrideCommandLineSwitchesForStaging) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kStaging;

  GlobalState::GetInstance()->Flags().did_override_from_command_line = false;

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsCommandLinePermissionRuleTest,
       ShouldNotAllowIfDidOverrideCommandLineSwitchesForProduction) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kProduction;

  GlobalState::GetInstance()->Flags().did_override_from_command_line = true;

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsCommandLinePermissionRuleTest,
       ShouldAllowIfDidOverrideCommandLineSwitchesForStaging) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kStaging;

  GlobalState::GetInstance()->Flags().did_override_from_command_line = true;

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
