/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(
      base::FeatureList::IsEnabled(CreativeAdModelBasedPredictorFeature));
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(
      base::FeatureList::IsEnabled(CreativeAdModelBasedPredictorFeature));
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     ChildIntentSegmentAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["child_intent_segment_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kChildIntentSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultChildIntentSegmentAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kChildIntentSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultChildIntentSegmentAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kChildIntentSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     ParentIntentSegmentAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["parent_intent_segment_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kParentIntentSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultParentIntentSegmentAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kParentIntentSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultParentIntentSegmentAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kParentIntentSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     ChildLatentInterestSegmentAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["child_latent_interest_segment_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kChildLatentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultChildLatentInterestSegmentAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kChildLatentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultChildLatentInterestSegmentAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kChildLatentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     ParentLatentInterestSegmentAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["parent_latent_interest_segment_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kParentLatentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultParentLatentInterestSegmentAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kParentLatentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultParentLatentInterestSegmentAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kParentLatentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     ChildInterestSegmentAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["child_interest_segment_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kChildInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultChildInterestSegmentAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kChildInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultChildInterestSegmentAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kChildInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     ParentInterestSegmentAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["parent_interest_segment_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kParentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultParentInterestSegmentAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kParentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultParentInterestSegmentAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kParentInterestSegmentAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     LastSeenAdAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["last_seen_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kLastSeenAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultLastSeenAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kLastSeenAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultLastSeenAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kLastSeenAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     LastSeenAdvertiserAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["last_seen_advertiser_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kLastSeenAdvertiserAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultLastSeenAdvertiserAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kLastSeenAdvertiserAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultLastSeenAdvertiserAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kLastSeenAdvertiserAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     PriorityAdPredictorWeight) {
  // Arrange
  base::FieldTrialParams params;
  params["priority_ad_predictor_weight"] = "0.5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(0.5, kPriorityAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultPriorityAdPredictorWeight) {
  // Arrange

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kPriorityAdPredictorWeight.Get());
}

TEST(HnsAdsCreativeAdModelBasedPredictorFeatureTest,
     DefaultPriorityAdPredictorWeightWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(CreativeAdModelBasedPredictorFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_DOUBLE_EQ(1.0, kPriorityAdPredictorWeight.Get());
}

}  // namespace hns_ads
