/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/dynamic/diagnostic_id_user_data.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsDiagnosticIdUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsDiagnosticIdUserDataTest,
       BuildDiagnosticIdUserDataForRewardsUser) {
  // Arrange
  SetStringPref(prefs::kDiagnosticId, "c1298fde-7fdb-401f-a3ce-0b58fe86e6e2");

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(
                R"({"diagnosticId":"c1298fde-7fdb-401f-a3ce-0b58fe86e6e2"})"),
            BuildDiagnosticIdUserData());
}

TEST_F(HnsAdsDiagnosticIdUserDataTest,
       BuildDiagnosticIdUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  SetStringPref(prefs::kDiagnosticId, "c1298fde-7fdb-401f-a3ce-0b58fe86e6e2");

  // Act

  // Assert
  EXPECT_TRUE(BuildDiagnosticIdUserData().empty());
}

TEST_F(HnsAdsDiagnosticIdUserDataTest,
       DoNotBuildDiagnosticUserDataIfDiagnosticIdIsInvalid) {
  // Arrange
  SetStringPref(prefs::kDiagnosticId, "INVALID");

  // Act

  // Assert
  EXPECT_TRUE(BuildDiagnosticIdUserData().empty());
}

TEST_F(HnsAdsDiagnosticIdUserDataTest,
       DoNotBuildDiagnosticIdUserDataIfDiagnosticIdIsEmpty) {
  // Arrange
  SetStringPref(prefs::kDiagnosticId, "");

  // Act

  // Assert
  EXPECT_TRUE(BuildDiagnosticIdUserData().empty());
}

}  // namespace hns_ads
