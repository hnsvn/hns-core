/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsPermissionRuleFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsPermissionRuleFeatureEnabled());
}

TEST(HnsAdsPermissionRuleFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPermissionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsPermissionRuleFeatureEnabled());
}

TEST(HnsAdsPermissionRuleFeatureTest, ShouldOnlyServeAdsInWindowedMode) {
  // Arrange
  base::FieldTrialParams params;
  params["should_only_serve_ads_in_windowed_mode"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPermissionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(kShouldOnlyServeAdsInWindowedMode.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsInWindowedMode) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsInWindowedMode.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsInWindowedModeWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPermissionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsInWindowedMode.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     ShouldOnlyServeAdsWithValidInternetConnection) {
  // Arrange
  base::FieldTrialParams params;
  params["should_only_serve_ads_with_valid_internet_connection"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPermissionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(kShouldOnlyServeAdsWithValidInternetConnection.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsWithValidInternetConnection) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsWithValidInternetConnection.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     ShouldOnlyServeAdsWithValidInternetConnectionWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPermissionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsWithValidInternetConnection.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest, ShouldOnlyServeAdsIfMediaIsNotPlaying) {
  // Arrange
  base::FieldTrialParams params;
  params["should_only_serve_ads_if_media_is_not_playing"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPermissionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(kShouldOnlyServeAdsIfMediaIsNotPlaying.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsIfMediaIsNotPlaying) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsIfMediaIsNotPlaying.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsIfMediaIsNotPlayingWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPermissionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsIfMediaIsNotPlaying.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest, ShouldOnlyServeAdsIfBrowserIsActive) {
  // Arrange
  base::FieldTrialParams params;
  params["should_only_serve_ads_if_browser_is_active"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPermissionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(kShouldOnlyServeAdsIfBrowserIsActive.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsIfBrowserIsActive) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsIfBrowserIsActive.Get());
}

TEST(HnsAdsPermissionRuleFeatureTest,
     DefaultShouldOnlyServeAdsIfBrowserIsActiveWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPermissionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(kShouldOnlyServeAdsIfBrowserIsActive.Get());
}

}  // namespace hns_ads
