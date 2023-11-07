/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_scoring_util.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_feature.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsUserActivityScoringUtilTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    base::FieldTrialParams params;
    params["triggers"] = "0D=1.0;08=1.0";
    params["time_window"] = "1h";
    params["threshold"] = "2.0";
    std::vector<base::test::FeatureRefAndParams> enabled_features;
    enabled_features.emplace_back(kUserActivityFeature, params);

    const std::vector<base::test::FeatureRef> disabled_features;

    scoped_feature_list_.InitWithFeaturesAndParameters(enabled_features,
                                                       disabled_features);
  }

  base::test::ScopedFeatureList scoped_feature_list_;
};

TEST_F(HnsAdsUserActivityScoringUtilTest, WasUserActive) {
  // Arrange
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);

  // Act
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);

  // Assert
  EXPECT_TRUE(WasUserActive());
}

TEST_F(HnsAdsUserActivityScoringUtilTest, WasUserInactive) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(WasUserActive());
}

TEST_F(HnsAdsUserActivityScoringUtilTest, WasUserInactiveIfBelowThreshold) {
  // Arrange

  // Act
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);

  // Assert
  EXPECT_FALSE(WasUserActive());
}

TEST_F(HnsAdsUserActivityScoringUtilTest,
       WasUserInactiveAfterTimeWindowHasElapsed) {
  // Arrange
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);

  // Act
  AdvanceClockBy(kUserActivityTimeWindow.Get() + base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(WasUserActive());
}

}  // namespace hns_ads
