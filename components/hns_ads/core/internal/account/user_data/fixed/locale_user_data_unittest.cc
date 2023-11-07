/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/locale_user_data.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsLocaleUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsLocaleUserDataTest,
       BuildLocaleUserDataForReleaseBuildChannelAndRewardsUser) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"countryCode":"US"})"),
            BuildLocaleUserData());
}

TEST_F(HnsAdsLocaleUserDataTest,
       DoNotBuildLocaleUserDataForReleaseBuildChannelForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(BuildLocaleUserData().empty());
}

TEST_F(HnsAdsLocaleUserDataTest,
       DoNotBuildLocaleUserDataForNonReleaseBuildChannel) {
  // Arrange
  MockBuildChannel(BuildChannelType::kNightly);

  // Act
  const base::Value::Dict user_data = BuildLocaleUserData();

  // Assert
  EXPECT_TRUE(user_data.empty());
}

TEST_F(HnsAdsLocaleUserDataTest,
       DoNotBuildLocaleUserDataForCountryNotInAnonymitySet) {
  // Arrange
  const hns_l10n::test::ScopedDefaultLocale scoped_default_locale{"en_MC"};

  // Act
  const base::Value::Dict user_data = BuildLocaleUserData();

  // Assert
  EXPECT_TRUE(user_data.empty());
}

TEST_F(HnsAdsLocaleUserDataTest,
       BuildLocaleUserDataForCountryNotInAnonymitySetButShouldClassifyAsOther) {
  // Arrange
  const hns_l10n::test::ScopedDefaultLocale scoped_default_locale{"en_CX"};

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"countryCode":"??"})"),
            BuildLocaleUserData());
}

}  // namespace hns_ads
