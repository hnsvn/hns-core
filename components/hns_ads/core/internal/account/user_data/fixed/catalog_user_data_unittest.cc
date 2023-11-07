/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/catalog_user_data.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCatalogUserDataTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    SetCatalogId(kCatalogId);
  }
};

TEST_F(HnsAdsCatalogUserDataTest, BuildCatalogUserDataForRewardsUser) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(
      base::test::ParseJsonDict(
          R"({"catalog":[{"id":"29e5c8bc0ba319069980bb390d8e8f9b58c05a20"}]})"),
      BuildCatalogUserData());
}

TEST_F(HnsAdsCatalogUserDataTest,
       DoNotBuildCatalogUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(BuildCatalogUserData().empty());
}

}  // namespace hns_ads
