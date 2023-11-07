/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/platform_user_data.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPlatformUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsPlatformUserDataTest, BuildPlatformUserDataForRewardsUser) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"platform":"windows"})"),
            BuildPlatformUserData());
}

TEST_F(HnsAdsPlatformUserDataTest,
       DoNotBuildPlatformUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(BuildPlatformUserData().empty());
}

}  // namespace hns_ads
