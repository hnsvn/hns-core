/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/client/legacy_client_migration_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsLegacyClientMigrationUtilTest : public UnitTestBase {};

TEST_F(HnsAdsLegacyClientMigrationUtilTest, HasMigrated) {
  // Arrange
  SetBooleanPref(prefs::kHasMigratedClientState, true);

  // Act

  // Assert
  EXPECT_TRUE(HasMigratedClientState());
}

TEST_F(HnsAdsLegacyClientMigrationUtilTest, HasNotMigrated) {
  // Arrange
  SetBooleanPref(prefs::kHasMigratedClientState, false);

  // Act

  // Assert
  EXPECT_FALSE(HasMigratedClientState());
}

}  // namespace hns_ads
