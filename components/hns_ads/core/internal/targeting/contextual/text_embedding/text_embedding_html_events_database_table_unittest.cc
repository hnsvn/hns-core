/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_events_database_table.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::database::table {

class HnsAdsTextEmbeddingHtmlEventsDatabaseTableTest : public UnitTestBase {};

TEST(HnsAdsTextEmbeddingHtmlEventsDatabaseTableTest, TableName) {
  // Arrange
  const TextEmbeddingHtmlEvents database_table;

  // Act

  // Assert
  EXPECT_EQ("text_embedding_html_events", database_table.GetTableName());
}

}  // namespace hns_ads::database::table
