/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/last_unidle_time_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsLastUnIdleTimeDiagnosticEntryTest : public UnitTestBase {};

TEST_F(HnsAdsLastUnIdleTimeDiagnosticEntryTest, LastUnIdleTime) {
  // Arrange
  AdvanceClockTo(
      TimeFromString("Mon, 8 July 1996 12:34:56", /*is_local*/ true));

  // Act
  LastUnIdleTimeDiagnosticEntry diagnostic_entry(Now());

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kLastUnIdleTime, diagnostic_entry.GetType());
  EXPECT_EQ("Last unidle time", diagnostic_entry.GetName());
  EXPECT_EQ("Monday, July 8, 1996 at 12:34:56\u202fPM",
            diagnostic_entry.GetValue());
}

TEST_F(HnsAdsLastUnIdleTimeDiagnosticEntryTest, WasNeverUnIdle) {
  // Arrange
  const LastUnIdleTimeDiagnosticEntry diagnostic_entry;

  // Act

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kLastUnIdleTime, diagnostic_entry.GetType());
  EXPECT_EQ("Last unidle time", diagnostic_entry.GetName());
  EXPECT_EQ("Never", diagnostic_entry.GetValue());
}

}  // namespace hns_ads
