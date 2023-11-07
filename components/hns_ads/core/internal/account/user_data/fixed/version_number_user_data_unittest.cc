/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/version_number_user_data.h"

#include <string>

#include "base/strings/string_util.h"
#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/browser/browser_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsVersionNumberUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsVersionNumberUserDataTest,
       BuildVersionNumberUserDataForRewardsUser) {
  // Arrange

  // Act

  // Assert
  const std::string expected_json = base::ReplaceStringPlaceholders(
      R"({"versionNumber":"$1"})", {GetBrowserVersionNumber()}, nullptr);
  EXPECT_EQ(base::test::ParseJsonDict(expected_json),
            BuildVersionNumberUserData());
}

TEST_F(HnsAdsVersionNumberUserDataTest,
       BuildVersionNumberUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(BuildVersionNumberUserData().empty());
}

}  // namespace hns_ads
