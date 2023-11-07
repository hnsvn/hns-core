/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/locale_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsLocaleDiagnosticEntryTest : public UnitTestBase {};

TEST_F(HnsAdsLocaleDiagnosticEntryTest, GetValue) {
  // Arrange
  const hns_l10n::test::ScopedDefaultLocale scoped_default_locale{"en_KY"};

  const LocaleDiagnosticEntry diagnostic_entry;

  // Act

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kLocale, diagnostic_entry.GetType());
  EXPECT_EQ("Locale", diagnostic_entry.GetName());
  EXPECT_EQ("en_KY", diagnostic_entry.GetValue());
}

}  // namespace hns_ads
