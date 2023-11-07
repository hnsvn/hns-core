/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/device_id_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"

// npm run test -- hns_unit_tests --filter=HnsAds.*

namespace hns_ads {

class HnsAdsDeviceIdDiagnosticEntryTest : public UnitTestBase {};

TEST_F(HnsAdsDeviceIdDiagnosticEntryTest, GetValue) {
  // Arrange
  MockDeviceId();

  // Act
  const DeviceIdDiagnosticEntry diagnostic_entry;

  // Assert
  EXPECT_EQ(DiagnosticEntryType::kDeviceId, diagnostic_entry.GetType());
  EXPECT_EQ("Device Id", diagnostic_entry.GetName());
  EXPECT_EQ(kDeviceId, diagnostic_entry.GetValue());
}

}  // namespace hns_ads
