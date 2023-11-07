/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsExclusionRuleFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsExclusionRuleFeatureEnabled());
}

TEST(HnsAdsExclusionRuleFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kExclusionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsExclusionRuleFeatureEnabled());
}

TEST(HnsAdsExclusionRuleFeatureTest, ShouldExcludeAdIfConverted) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_converted"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(kShouldExcludeAdIfConverted.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest, DefaultShouldExcludeAdIfConverted) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(kShouldExcludeAdIfConverted.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     DefaultShouldExcludeAdIfConvertedWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kExclusionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(kShouldExcludeAdIfConverted.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     GetExcludeAdIfDismissedWithinTimeWindow) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "24h";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Days(1), kShouldExcludeAdIfDismissedWithinTimeWindow.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfDismissedWithinTimeWindow) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0), kShouldExcludeAdIfDismissedWithinTimeWindow.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfDismissedWithinTimeWindowWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kExclusionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0), kShouldExcludeAdIfDismissedWithinTimeWindow.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     GetExcludeAdIfTransferredWithinTimeWindow) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_transferred_within_time_window"] = "24h";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Days(1), kShouldExcludeAdIfTransferredWithinTimeWindow.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfTransferredWithinTimeWindow) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0),
            kShouldExcludeAdIfTransferredWithinTimeWindow.Get());
}

TEST(HnsAdsExclusionRuleFeatureTest,
     DefaultExcludeAdIfTransferredWithinTimeWindowWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kExclusionRulesFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Hours(0),
            kShouldExcludeAdIfTransferredWithinTimeWindow.Get());
}

}  // namespace hns_ads
