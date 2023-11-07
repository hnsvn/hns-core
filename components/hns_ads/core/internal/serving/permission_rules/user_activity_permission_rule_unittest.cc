/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/user_activity_permission_rule.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_feature.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsUserActivityPermissionRuleTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    base::FieldTrialParams params;
    params["triggers"] = "0D=1.0;0E=1.0;08=1.0";
    params["time_window"] = "1h";
    params["threshold"] = "2.0";
    std::vector<base::test::FeatureRefAndParams> enabled_features;
    enabled_features.emplace_back(kUserActivityFeature, params);

    const std::vector<base::test::FeatureRef> disabled_features;

    scoped_feature_list_.InitWithFeaturesAndParameters(enabled_features,
                                                       disabled_features);
  }

  base::test::ScopedFeatureList scoped_feature_list_;

  const UserActivityPermissionRule permission_rule_;
};

TEST_F(HnsAdsUserActivityPermissionRuleTest,
       ShouldAllowIfUserActivityScoreIsEqualToTheThreshold) {
  // Arrange

  // Act
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsUserActivityPermissionRuleTest,
       ShouldAllowIfUserHasNotJoinedHnsRewards) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsUserActivityPermissionRuleTest,
       ShouldAllowIfUserActivityScoreIsGreaterThanTheThreshold) {
  // Arrange

  // Act
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsUserActivityPermissionRuleTest,
       ShouldNotAllowIfUserActivityScoreIsLessThanTheThreshold) {
  // Arrange

  // Act
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
