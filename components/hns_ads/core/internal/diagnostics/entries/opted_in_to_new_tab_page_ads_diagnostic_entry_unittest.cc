/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/opted_in_to_new_tab_page_ads_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsOptedInToNewTabPageAdsDiagnosticEntryTest : public UnitTestBase {
};

TEST_F(HnsAdsOptedInToNewTabPageAdsDiagnosticEntryTest, IsOptedIn) {
  // Arrange

  // Act
  const OptedInToNewTabPageAdsDiagnosticEntry diagnostic_entry;

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kOptedInToNewTabPageAds,
            diagnostic_entry.GetType());
  EXPECT_EQ("Opted-in to new tab page ads", diagnostic_entry.GetName());
  EXPECT_EQ("true", diagnostic_entry.GetValue());
}

TEST_F(HnsAdsOptedInToNewTabPageAdsDiagnosticEntryTest, IsOptedOut) {
  // Arrange
  DisableNewTabPageAdsForTesting();

  // Act
  const OptedInToNewTabPageAdsDiagnosticEntry diagnostic_entry;

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kOptedInToNewTabPageAds,
            diagnostic_entry.GetType());
  EXPECT_EQ("Opted-in to new tab page ads", diagnostic_entry.GetName());
  EXPECT_EQ("false", diagnostic_entry.GetValue());
}

}  // namespace hns_ads
