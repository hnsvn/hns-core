/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/strings/stringprintf.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_constants.h"
#include "hns/components/hns_ads/core/internal/database/database_manager.h"
#include "hns/components/hns_ads/core/internal/database/database_manager_observer.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/internal/legacy_migration/database/database_constants.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

std::string TestParamToString(::testing::TestParamInfo<int> test_param) {
  return base::StringPrintf("%d_to_%d", test_param.param, database::kVersion);
}

}  // namespace

class HnsAdsInvalidDatabaseMigrationTest
    : public UnitTestBase,
      public ::testing::WithParamInterface<int>,
      public DatabaseManagerObserver {
 protected:
  void SetUpMocks() override {
    MaybeMockDatabase();

    GlobalState::GetInstance()->GetDatabaseManager().AddObserver(this);
  }

  void TearDown() override {
    GlobalState::GetInstance()->GetDatabaseManager().RemoveObserver(this);

    UnitTestBase::TearDown();
  }

  static int GetSchemaVersion() { return GetParam(); }

  void MaybeMockDatabase() {
    const std::string database_filename = base::StringPrintf(
        "database/invalid_database_schema_%d.sqlite", GetSchemaVersion());
    ASSERT_TRUE(
        CopyFileFromTestPathToTempPath(database_filename, kDatabaseFilename));
  }

  // DatabaseManagerObserver:
  void OnDidCreateDatabase() override { did_create_database_ = true; }

  void OnDidMigrateDatabase(const int /*from_version*/,
                            const int /*to_version*/) override {
    did_migrate_database_ = true;
  }

  void OnFailedToMigrateDatabase(const int /*from_version*/,
                                 const int /*to_version*/) override {
    failed_to_migrate_database_ = true;
  }

  void OnDatabaseIsReady() override { database_is_ready_ = true; }

  bool did_create_database_ = false;
  bool did_migrate_database_ = false;
  bool failed_to_migrate_database_ = false;
  bool database_is_ready_ = false;
};

TEST_P(HnsAdsInvalidDatabaseMigrationTest, MigrateFromSchema) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(did_create_database_);
  EXPECT_FALSE(did_migrate_database_);
  EXPECT_FALSE(failed_to_migrate_database_);
  EXPECT_TRUE(database_is_ready_);
}

INSTANTIATE_TEST_SUITE_P(,
                         HnsAdsInvalidDatabaseMigrationTest,
                         ::testing::Range(1, database::kVersion + 1),
                         TestParamToString);

}  // namespace hns_ads
