/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/client/legacy_client_migration.h"

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_state_manager_constants.h"
#include "hns/components/hns_ads/core/internal/legacy_migration/client/legacy_client_migration_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {
constexpr char kInvalidJsonFilename[] = "invalid.json";
}  // namespace

class HnsAdsLegacyClientMigrationTest : public UnitTestBase {
 protected:
  void SetUpMocks() override {
    SetBooleanPref(prefs::kHasMigratedClientState, false);
  }
};

TEST_F(HnsAdsLegacyClientMigrationTest, Migrate) {
  // Arrange
  ASSERT_TRUE(CopyFileFromTestPathToTempPath(kClientStateFilename));

  // Assert
  base::MockCallback<InitializeCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](const bool success) {
    EXPECT_TRUE(success);
    EXPECT_TRUE(HasMigratedClientState());
  });

  // Act
  MigrateClientState(callback.Get());
}

TEST_F(HnsAdsLegacyClientMigrationTest, InvalidState) {
  // Arrange
  ASSERT_TRUE(CopyFileFromTestPathToTempPath(kInvalidJsonFilename,
                                             kClientStateFilename));

  // Assert
  base::MockCallback<InitializeCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](const bool success) {
    EXPECT_FALSE(success);
    EXPECT_FALSE(HasMigratedClientState());
  });

  // Act
  MigrateClientState(callback.Get());
}

}  // namespace hns_ads
