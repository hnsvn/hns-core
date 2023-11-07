/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/confirmations/legacy_confirmation_migration_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsLegacyConfirmationMigrationUtilTest : public UnitTestBase {};

TEST_F(HnsAdsLegacyConfirmationMigrationUtilTest, HasMigrated) {
  // Arrange
  SetBooleanPref(prefs::kHasMigratedConfirmationState, true);

  // Act

  // Assert
  EXPECT_TRUE(HasMigratedConfirmation());
}

TEST_F(HnsAdsLegacyConfirmationMigrationUtilTest, HasNotMigrated) {
  // Arrange
  SetBooleanPref(prefs::kHasMigratedConfirmationState, false);

  // Act

  // Assert
  EXPECT_FALSE(HasMigratedConfirmation());
}

}  // namespace hns_ads
