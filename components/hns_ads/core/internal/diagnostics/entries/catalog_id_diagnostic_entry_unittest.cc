/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/catalog_id_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsCatalogIdDiagnosticEntryTest : public UnitTestBase {};

TEST_F(HnsAdsCatalogIdDiagnosticEntryTest, CatalogId) {
  // Arrange
  SetCatalogId(kCatalogId);

  // Act
  const CatalogIdDiagnosticEntry diagnostic_entry;

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kCatalogId, diagnostic_entry.GetType());
  EXPECT_EQ("Catalog ID", diagnostic_entry.GetName());
  EXPECT_EQ(kCatalogId, diagnostic_entry.GetValue());
}

TEST_F(HnsAdsCatalogIdDiagnosticEntryTest, EmptyCatalogId) {
  // Arrange

  // Act
  const CatalogIdDiagnosticEntry diagnostic_entry;

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kCatalogId, diagnostic_entry.GetType());
  EXPECT_EQ("Catalog ID", diagnostic_entry.GetName());
  EXPECT_EQ("", diagnostic_entry.GetValue());
}

}  // namespace hns_ads
