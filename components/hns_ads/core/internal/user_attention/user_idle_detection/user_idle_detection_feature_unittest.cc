/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsUserIdleDetectionFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsUserIdleDetectionFeatureEnabled());
}

TEST(HnsAdsUserIdleDetectionFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kUserIdleDetectionFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsUserIdleDetectionFeatureEnabled());
}

TEST(HnsAdsUserIdleDetectionFeatureTest, GetMaximumIdleTime) {
  // Arrange
  base::FieldTrialParams params;
  params["maximum_idle_time"] = "30m";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kUserIdleDetectionFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(30), kMaximumIdleTime.Get());
}

TEST(HnsAdsUserIdleDetectionFeatureTest, DefaultMaximumIdleTime) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Seconds(0), kMaximumIdleTime.Get());
}

TEST(HnsAdsUserIdleDetectionFeatureTest, DefaultMaximumIdleTimeWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kUserIdleDetectionFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Seconds(0), kMaximumIdleTime.Get());
}

TEST(HnsAdsUserIdleDetectionFeatureTest, ShouldDetectScreenWasLocked) {
  // Arrange
  base::FieldTrialParams params;
  params["should_detect_screen_was_locked"] = "true";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kUserIdleDetectionFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(kShouldDetectScreenWasLocked.Get());
}

TEST(HnsAdsUserIdleDetectionFeatureTest, DefaultShouldDetectScreenWasLocked) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(kShouldDetectScreenWasLocked.Get());
}

TEST(HnsAdsUserIdleDetectionFeatureTest,
     ShouldDetectScreenWasLockedWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kUserIdleDetectionFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(kShouldDetectScreenWasLocked.Get());
}

}  // namespace hns_ads
