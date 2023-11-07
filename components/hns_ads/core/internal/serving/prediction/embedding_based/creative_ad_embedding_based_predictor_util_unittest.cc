/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/embedding_based/creative_ad_embedding_based_predictor_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorUtilTest,
     CalculateNormalizingConstantForVoteRegistry) {
  // Arrange
  const std::vector<int> creative_ad_vote_registry = {1, 2, 3};

  // Act
  const double normalizing_constant =
      CalculateNormalizingConstantForVoteRegistry(creative_ad_vote_registry);

  // Assert
  EXPECT_DOUBLE_EQ(6.0, normalizing_constant);
}

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorUtilTest,
     CalculateNormalizingConstantForEmptyVoteRegistry) {
  // Arrange
  const std::vector<int> creative_ad_vote_registry;

  // Act
  const double normalizing_constant =
      CalculateNormalizingConstantForVoteRegistry(creative_ad_vote_registry);

  // Assert
  EXPECT_DOUBLE_EQ(0.0, normalizing_constant);
}

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorUtilTest,
     ComputeCreativeAdProbabilitiesForVoteRegistry) {
  // Arrange
  const std::vector<int> creative_ad_vote_registry = {1, 2, 3};

  // Act
  const std::vector<double> creative_ad_propabilities =
      ComputeCreativeAdProbabilitiesForVoteRegistry(creative_ad_vote_registry);

  // Assert
  EXPECT_DOUBLE_EQ(0.16666666666666666, creative_ad_propabilities.at(0));
  EXPECT_DOUBLE_EQ(0.33333333333333331, creative_ad_propabilities.at(1));
  EXPECT_DOUBLE_EQ(0.5, creative_ad_propabilities.at(2));
}

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorUtilTest,
     ComputeCreativeAdProbabilitiesForEmptyVoteRegistry) {
  // Arrange
  const std::vector<int> creative_ad_vote_registry;

  // Act
  const std::vector<double> creative_ad_propabilities =
      ComputeCreativeAdProbabilitiesForVoteRegistry(creative_ad_vote_registry);

  // Assert
  EXPECT_TRUE(creative_ad_propabilities.empty());
}

}  // namespace hns_ads
