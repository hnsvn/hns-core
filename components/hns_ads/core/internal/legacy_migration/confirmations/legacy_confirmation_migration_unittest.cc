/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/confirmations/legacy_confirmation_migration.h"

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/deprecated/confirmations/confirmation_state_manager_constants.h"
#include "hns/components/hns_ads/core/internal/legacy_migration/confirmations/legacy_confirmation_migration_util.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {
constexpr char kInvalidJsonFilename[] = "invalid.json";
}  // namespace

class HnsAdsLegacyConfirmationMigrationTest : public UnitTestBase {
 protected:
  void SetUpMocks() override {
    SetBooleanPref(prefs::kHasMigratedConfirmationState, false);
  }
};

TEST_F(HnsAdsLegacyConfirmationMigrationTest, Migrate) {
  // Arrange
  ASSERT_TRUE(CopyFileFromTestPathToTempPath(kConfirmationStateFilename));

  // Assert
  base::MockCallback<InitializeCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](const bool success) {
    EXPECT_TRUE(success);
    EXPECT_TRUE(HasMigratedConfirmation());
  });

  // Act
  MigrateConfirmationState(callback.Get());
}

TEST_F(HnsAdsLegacyConfirmationMigrationTest, InvalidState) {
  // Arrange
  ASSERT_TRUE(CopyFileFromTestPathToTempPath(kInvalidJsonFilename,
                                             kConfirmationStateFilename));

  // Assert
  base::MockCallback<InitializeCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](const bool success) {
    EXPECT_FALSE(success);
    EXPECT_FALSE(HasMigratedConfirmation());
  });

  // Act
  MigrateConfirmationState(callback.Get());
}

}  // namespace hns_ads
