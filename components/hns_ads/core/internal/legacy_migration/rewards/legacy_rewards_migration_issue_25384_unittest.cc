/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/rewards/legacy_rewards_migration.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/deprecated/confirmations/confirmation_state_manager_constants.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {
constexpr char kIssue25384ConfirmationStateFilename[] =
    "confirmations_issue_25384.json";
}  // namespace

class HnsAdsLegacyRewardsMigrationIssue25384Test : public UnitTestBase {
 protected:
  void SetUpMocks() override {
    ASSERT_TRUE(CopyFileFromTestPathToTempPath(
        kIssue25384ConfirmationStateFilename, kConfirmationStateFilename));
  }
};

TEST_F(HnsAdsLegacyRewardsMigrationIssue25384Test, Migrate) {
  // Arrange
  SetBooleanPref(prefs::kHasMigratedRewardsState, false);

  EXPECT_CALL(ads_client_mock_, Load(kConfirmationStateFilename, ::testing::_));

  // Act
  rewards::Migrate(
      base::BindOnce([](const bool success) { ASSERT_TRUE(success); }));

  // Assert
  EXPECT_TRUE(ads_client_mock_.GetBooleanPref(prefs::kHasMigratedRewardsState));
}

}  // namespace hns_ads
