/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/catalog_last_updated_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsCatalogLastUpdatedDiagnosticEntryTest : public UnitTestBase {};

TEST_F(HnsAdsCatalogLastUpdatedDiagnosticEntryTest, CatalogLastUpdated) {
  // Arrange
  AdvanceClockTo(
      TimeFromString("Wed, 18 Nov 1970 12:34:56", /*is_local*/ true));

  SetCatalogLastUpdated(Now());

  const CatalogLastUpdatedDiagnosticEntry diagnostic_entry;

  // Act

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kCatalogLastUpdated,
            diagnostic_entry.GetType());
  EXPECT_EQ("Catalog last updated", diagnostic_entry.GetName());
  EXPECT_EQ("Wednesday, November 18, 1970 at 12:34:56\u202fPM",
            diagnostic_entry.GetValue());
}

TEST_F(HnsAdsCatalogLastUpdatedDiagnosticEntryTest, CatalogNeverUpdated) {
  // Arrange
  const CatalogLastUpdatedDiagnosticEntry diagnostic_entry;

  // Act

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kCatalogLastUpdated,
            diagnostic_entry.GetType());
  EXPECT_EQ("Catalog last updated", diagnostic_entry.GetName());
  EXPECT_EQ("", diagnostic_entry.GetValue());
}

}  // namespace hns_ads
