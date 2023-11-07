/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/geo_targets_database_table.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::database::table {

TEST(HnsAdsGeoTargetsDatabaseTableTest, TableName) {
  // Arrange
  const GeoTargets database_table;

  // Act

  // Assert
  EXPECT_EQ("geo_targets", database_table.GetTableName());
}

}  // namespace hns_ads::database::table
