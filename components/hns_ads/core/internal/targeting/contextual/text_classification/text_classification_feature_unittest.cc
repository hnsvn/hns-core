/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/text_classification_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsTextClassificationFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsTextClassificationFeatureEnabled());
}

TEST(HnsAdsTextClassificationFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kTextClassificationFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsTextClassificationFeatureEnabled());
}

TEST(HnsAdsTextClassificationFeatureTest,
     GetTextClassificationResourceVersion) {
  // Arrange
  base::FieldTrialParams params;
  params["resource_version"] = "0";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kTextClassificationFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(0, kTextClassificationResourceVersion.Get());
}

TEST(HnsAdsTextClassificationFeatureTest,
     DefaultTextClassificationResourceVersion) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(1, kTextClassificationResourceVersion.Get());
}

TEST(HnsAdsTextClassificationFeatureTest,
     DefaultTextClassificationResourceVersionWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kTextClassificationFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(1, kTextClassificationResourceVersion.Get());
}

TEST(HnsAdsTextClassificationFeatureTest,
     GetTextClassificationProbabilitiesHistorySize) {
  // Arrange
  base::FieldTrialParams params;
  params["page_probabilities_history_size"] = "3";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kTextClassificationFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(3, kTextClassificationPageProbabilitiesHistorySize.Get());
}

TEST(HnsAdsTextClassificationFeatureTest,
     DefaultTextClassificationProbabilitiesHistorySize) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(5, kTextClassificationPageProbabilitiesHistorySize.Get());
}

TEST(HnsAdsTextClassificationFeatureTest,
     DefaultTextClassificationProbabilitiesHistorySizeWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kTextClassificationFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(5, kTextClassificationPageProbabilitiesHistorySize.Get());
}

}  // namespace hns_ads
