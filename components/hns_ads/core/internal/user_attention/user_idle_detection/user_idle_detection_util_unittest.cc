/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection_util.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection_feature.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsUserIdleDetectionUtilTest : public UnitTestBase {};

TEST_F(HnsAdsUserIdleDetectionUtilTest, WasLocked) {
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
  EXPECT_TRUE(MaybeScreenWasLocked(/*screen_was_locked*/ true));
}

TEST_F(HnsAdsUserIdleDetectionUtilTest,
       WasLockedIfShouldDetectScreenWasLocked) {
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
  EXPECT_TRUE(MaybeScreenWasLocked(/*screen_was_locked*/ true));
}

TEST_F(HnsAdsUserIdleDetectionUtilTest, WasNotLocked) {
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
  EXPECT_FALSE(MaybeScreenWasLocked(/*screen_was_locked*/ false));
}

TEST_F(HnsAdsUserIdleDetectionUtilTest,
       WasNotLockedIfShouldNotDetectWasLocked) {
  // Arrange
  base::FieldTrialParams params;
  params["should_detect_screen_was_locked"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kUserIdleDetectionFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(MaybeScreenWasLocked(/*screen_was_locked*/ true));
}

TEST_F(HnsAdsUserIdleDetectionUtilTest, HasNotExceededMaximumIdleTime) {
  // Arrange
  base::FieldTrialParams params;
  params["maximum_idle_time"] = "10s";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kUserIdleDetectionFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(HasExceededMaximumIdleTime(base::Seconds(10)));
}

TEST_F(HnsAdsUserIdleDetectionUtilTest,
       HasNotExceededInfiniteMaximumIdleTime) {
  // Arrange
  base::FieldTrialParams params;
  params["maximum_idle_time"] = "0s";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kUserIdleDetectionFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(HasExceededMaximumIdleTime(base::TimeDelta::Max()));
}

TEST_F(HnsAdsUserIdleDetectionUtilTest, HasExceededMaximumIdleTime) {
  // Arrange
  base::FieldTrialParams params;
  params["maximum_idle_time"] = "10s";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kUserIdleDetectionFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(HasExceededMaximumIdleTime(base::Seconds(11)));
}

}  // namespace hns_ads
