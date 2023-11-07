/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/scoring/creative_ad_model_based_predictor_scoring_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_segment_input_variable_info.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeMatchingChildIntentSegmentScore) {
  // Arrange
  CreativeAdPredictorSegmentInputVariableInfo input_variable;
  input_variable.does_match_child = true;

  // Act
  const double score = ComputeIntentSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeMatchingParentIntentSegmentScore) {
  // Arrange
  CreativeAdPredictorSegmentInputVariableInfo input_variable;
  input_variable.does_match_parent = true;

  // Act
  const double score = ComputeIntentSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeNonMatchingIntentSegmentScore) {
  // Arrange
  const CreativeAdPredictorSegmentInputVariableInfo input_variable;

  // Act
  const double score = ComputeIntentSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(0.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeMatchingChildLatentInterestSegmentScore) {
  // Arrange
  CreativeAdPredictorSegmentInputVariableInfo input_variable;
  input_variable.does_match_child = true;

  // Act
  const double score = ComputeLatentInterestSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeMatchingParentLatentInterestSegmentScore) {
  // Arrange
  CreativeAdPredictorSegmentInputVariableInfo input_variable;
  input_variable.does_match_parent = true;

  // Act
  const double score = ComputeLatentInterestSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeNonMatchingLatentInterestSegmentScore) {
  // Arrange
  const CreativeAdPredictorSegmentInputVariableInfo input_variable;

  // Act
  const double score = ComputeLatentInterestSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(0.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeMatchingChildInterestSegmentScore) {
  // Arrange
  CreativeAdPredictorSegmentInputVariableInfo input_variable;
  input_variable.does_match_child = true;

  // Act
  const double score = ComputeInterestSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeMatchingParentInterestSegmentScore) {
  // Arrange
  CreativeAdPredictorSegmentInputVariableInfo input_variable;
  input_variable.does_match_parent = true;

  // Act
  const double score = ComputeInterestSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeNonMatchingInterestSegmentScore) {
  // Arrange
  const CreativeAdPredictorSegmentInputVariableInfo input_variable;

  // Act
  const double score = ComputeInterestSegmentScore(input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(0.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest, ComputeLastSeenAdScore) {
  // Arrange
  const base::TimeDelta last_seen = base::Hours(7);

  // Act
  const double score = ComputeLastSeenAdScore(last_seen);

  // Assert
  EXPECT_DOUBLE_EQ(0.29166666666666669, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest, ComputeNeverSeenAdScore) {
  // Arrange
  const absl::optional<base::TimeDelta> last_seen;

  // Act
  const double score = ComputeLastSeenAdScore(last_seen);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeLastSeenAdScoreIfExceeds1Day) {
  // Arrange
  const base::TimeDelta last_seen = base::Days(1) + base::Milliseconds(1);

  // Act
  const double score = ComputeLastSeenAdScore(last_seen);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeLastSeenAdvertiserScore) {
  // Arrange
  const base::TimeDelta last_seen = base::Hours(7);

  // Act
  const double score = ComputeLastSeenAdvertiserScore(last_seen);

  // Assert
  EXPECT_DOUBLE_EQ(0.29166666666666669, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeNeverSeenAdvertiserScore) {
  // Arrange
  const absl::optional<base::TimeDelta> last_seen;

  // Act
  const double score = ComputeLastSeenAdvertiserScore(last_seen);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest,
     ComputeLastSeenAdvertiserScoreIfExceeds1Day) {
  // Arrange
  const base::TimeDelta last_seen = base::Days(1) + base::Milliseconds(1);

  // Act
  const double score = ComputeLastSeenAdvertiserScore(last_seen);

  // Assert
  EXPECT_DOUBLE_EQ(1.0, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest, ComputePriorityScore) {
  // Arrange
  const int priority = 5;

  // Act
  const double score = ComputePriorityScore(priority);

  // Assert
  EXPECT_DOUBLE_EQ(0.2, score);
}

TEST(HnsAdsCreativeAdPredictorScoringUtilTest, ComputeZeroPriorityScore) {
  // Arrange
  const int priority = 0;

  // Act
  const double score = ComputePriorityScore(priority);

  // Assert
  EXPECT_DOUBLE_EQ(0.0, score);
}

}  // namespace hns_ads
