/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCatalogUtilTest : public UnitTestBase {};

TEST_F(HnsAdsCatalogUtilTest, ResetCatalog) {
  // Arrange
  SetCatalogId(kCatalogId);
  SetCatalogVersion(1);
  SetCatalogPing(base::Hours(1));
  SetCatalogLastUpdated(Now());

  // Act
  ResetCatalog();

  // Assert
  EXPECT_EQ("", ads_client_mock_.GetStringPref(prefs::kCatalogId));
  EXPECT_EQ(0, ads_client_mock_.GetIntegerPref(prefs::kCatalogVersion));
  EXPECT_EQ(7'200'000, ads_client_mock_.GetInt64Pref(prefs::kCatalogPing));
  EXPECT_TRUE(
      ads_client_mock_.GetTimePref(prefs::kCatalogLastUpdated).is_null());
}

TEST_F(HnsAdsCatalogUtilTest, CatalogExists) {
  // Arrange
  SetCatalogVersion(1);

  // Act

  // Assert
  EXPECT_TRUE(DoesCatalogExist());
}

TEST_F(HnsAdsCatalogUtilTest, CatalogDoesNotExist) {
  // Arrange
  SetCatalogVersion(0);

  // Act

  // Assert
  EXPECT_FALSE(DoesCatalogExist());
}

TEST_F(HnsAdsCatalogUtilTest, CatalogHasChanged) {
  // Arrange
  SetCatalogId(kCatalogId);

  // Act

  // Assert
  EXPECT_TRUE(
      HasCatalogChanged(/*catalog_id*/ "150a9518-4db8-4fba-b104-0c420a1d9c0c"));
}

TEST_F(HnsAdsCatalogUtilTest, CatalogHasNotChanged) {
  // Arrange
  SetCatalogId(kCatalogId);

  // Act

  // Assert
  EXPECT_FALSE(HasCatalogChanged(kCatalogId));
}

TEST_F(HnsAdsCatalogUtilTest, CatalogHasExpired) {
  // Arrange
  SetCatalogLastUpdated(Now());

  // Act
  AdvanceClockBy(base::Days(1));

  // Assert
  EXPECT_TRUE(HasCatalogExpired());
}

TEST_F(HnsAdsCatalogUtilTest, CatalogHasNotExpired) {
  // Arrange
  SetCatalogLastUpdated(Now());

  // Act
  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(HasCatalogExpired());
}

}  // namespace hns_ads
