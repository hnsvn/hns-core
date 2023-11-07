/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsTextEmbeddingFeatureTest, IsEnabled) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(kTextEmbeddingFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(IsTextEmbeddingFeatureEnabled());
}

TEST(HnsAdsTextEmbeddingFeatureTest, IsDisabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsTextEmbeddingFeatureEnabled());
}

TEST(HnsAdsTextEmbeddingFeatureTest, GetTextEmbeddingResourceVersion) {
  // Arrange
  base::FieldTrialParams params;
  params["resource_version"] = "0";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kTextEmbeddingFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(0, kTextEmbeddingResourceVersion.Get());
}

TEST(HnsAdsTextEmbeddingFeatureTest, DefaultTextEmbeddingResourceVersion) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(1, kTextEmbeddingResourceVersion.Get());
}

TEST(HnsAdsTextEmbeddingFeatureTest,
     DefaultTextEmbeddingResourceVersionWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kTextEmbeddingFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(1, kTextEmbeddingResourceVersion.Get());
}

TEST(HnsAdsTextEmbeddingFeatureTest, GetTextEmbeddingsHistorySize) {
  // Arrange
  base::FieldTrialParams params;
  params["history_size"] = "42";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kTextEmbeddingFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(42, kTextEmbeddingHistorySize.Get());
}

TEST(HnsAdsTextEmbeddingFeatureTest, DefaultTextEmbeddingsHistorySize) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(10, kTextEmbeddingHistorySize.Get());
}

TEST(HnsAdsTextClassificationFeatureTest,
     DefaultTextEmbeddingsHistorySizeWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kTextEmbeddingFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(10, kTextEmbeddingHistorySize.Get());
}

}  // namespace hns_ads
