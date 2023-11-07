/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <string>

#include "base/strings/string_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_command_line_switch_info.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_command_line_switch_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_command_line_switch_util_constants.h"
#include "hns/components/hns_ads/core/internal/flags/environment/environment_types_unittest_util.h"
#include "hns/components/hns_ads/core/internal/flags/flag_constants.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

struct ParamInfo final {
  CommandLineSwitchInfo command_line_switch;
  mojom::EnvironmentType expected_environment_type;
} const kTests[] = {
    {{kRewardsSwitch, "staging=true"}, mojom::EnvironmentType::kStaging},
    {{kRewardsSwitch, "staging=1"}, mojom::EnvironmentType::kStaging},
    {{kRewardsSwitch, "staging=false"}, mojom::EnvironmentType::kProduction},
    {{kRewardsSwitch, "staging=foobar"}, mojom::EnvironmentType::kProduction},
    {{}, kDefaultEnvironmentType}};

}  // namespace

class HnsAdsEnvironmentCommandLineSwitchParserUtilTest
    : public UnitTestBase,
      public ::testing::WithParamInterface<ParamInfo> {
 protected:
  void SetUpMocks() override {
    AppendCommandLineSwitches({GetParam().command_line_switch});
  }
};

TEST_P(HnsAdsEnvironmentCommandLineSwitchParserUtilTest,
       ParseEnvironmentCommandLineSwitch) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(GetParam().expected_environment_type,
            GlobalState::GetInstance()->Flags().environment_type);
}

std::string TestParamToString(
    const ::testing::TestParamInfo<ParamInfo>& test_param) {
  const std::string expected_environment_type =
      EnvironmentTypeEnumToStringForTesting(
          test_param.param.expected_environment_type);

  const std::string sanitized_command_line_switch =
      SanitizeCommandLineSwitch(test_param.param.command_line_switch);

  return base::ReplaceStringPlaceholders(
      "$1EnvironmentFor$2",
      {expected_environment_type, sanitized_command_line_switch}, nullptr);
}

INSTANTIATE_TEST_SUITE_P(,
                         HnsAdsEnvironmentCommandLineSwitchParserUtilTest,
                         ::testing::ValuesIn(kTests),
                         TestParamToString);

}  // namespace hns_ads
