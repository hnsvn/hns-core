/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_manager.h"

#include "base/test/values_test_util.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/last_unidle_time_diagnostic_util.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsDiagnosticManagerTest : public UnitTestBase {};

TEST_F(HnsAdsDiagnosticManagerTest, DiagnosticManager) {
  // Arrange
  MockDeviceId();

  const hns_l10n::test::ScopedDefaultLocale scoped_default_locale{"en_KY"};

  AdvanceClockTo(
      TimeFromString("Wed, 18 Nov 1970 12:34:56", /*is_local*/ true));

  SetCatalogId(kCatalogId);
  SetCatalogLastUpdated(Now());

  AdvanceClockTo(TimeFromString("Mon, 8 Jul 1996 09:25:00", /*is_local*/ true));

  SetLastUnIdleTimeDiagnosticEntry(Now());

  // Act
  DiagnosticManager::GetInstance().GetDiagnostics(
      base::BindOnce([](absl::optional<base::Value::List> list) {
        // Assert
        ASSERT_TRUE(list);

        const base::Value::List expected_list = base::test::ParseJsonList(
            R"([
                {
                  "name": "Device Id",
                  "value": "21b4677de1a9b4a197ab671a1481d3fcb24f826a4358a05aafbaee5a9a51b57e"
                },
                {
                  "name": "Opted-in to Hns News ads",
                  "value": "true"
                },
                {
                  "name": "Opted-in to new tab page ads",
                  "value": "true"
                },
                {
                  "name": "Opted-in to notification ads",
                  "value": "true"
                },
                {
                  "name": "Locale",
                  "value": "en_KY"
                },
                {
                  "name": "Catalog ID",
                  "value": "29e5c8bc0ba319069980bb390d8e8f9b58c05a20"
                },
                {
                  "name": "Catalog last updated",
                  "value": "Wednesday, November 18, 1970 at 12:34:56\u202fPM"
                },
                {
                  "name": "Last unidle time",
                  "value": "Monday, July 8, 1996 at 9:25:00\u202fAM"
                }
              ])");

        EXPECT_EQ(expected_list, list);
      }));
}

}  // namespace hns_ads
