/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_input_variable_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeAdPredictorInputVariableUtilTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorMatchingChildIntentSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.intent_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorIntentSegmentInputVariable(user_model,
                                                           "parent-child");

  // Assert
  EXPECT_TRUE(input_variable.does_match_child);
  EXPECT_TRUE(input_variable.does_match_parent);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorMatchingParentIntentSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.intent_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorIntentSegmentInputVariable(user_model,
                                                           "parent-foo");

  // Assert
  EXPECT_FALSE(input_variable.does_match_child);
  EXPECT_TRUE(input_variable.does_match_parent);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorNonMatchingIntentSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.intent_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorIntentSegmentInputVariable(user_model,
                                                           "foo-bar");

  // Assert
  EXPECT_FALSE(input_variable.does_match_child);
  EXPECT_FALSE(input_variable.does_match_parent);
}

TEST_F(
    HnsAdsCreativeAdPredictorInputVariableUtilTest,
    ComputeCreativeAdPredictorMatchingChildLatentInterestSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.latent_interest_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorLatentInterestSegmentInputVariable(
          user_model, "parent-child");

  // Assert
  EXPECT_TRUE(input_variable.does_match_child);
  EXPECT_TRUE(input_variable.does_match_parent);
}

TEST_F(
    HnsAdsCreativeAdPredictorInputVariableUtilTest,
    ComputeCreativeAdPredictorMatchingParentLatentInterestSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.latent_interest_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorLatentInterestSegmentInputVariable(
          user_model, "parent-foo");

  // Assert
  EXPECT_FALSE(input_variable.does_match_child);
  EXPECT_TRUE(input_variable.does_match_parent);
}

TEST_F(
    HnsAdsCreativeAdPredictorInputVariableUtilTest,
    ComputeCreativeAdPredictorNonMatchingLatentInterestSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.latent_interest_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorLatentInterestSegmentInputVariable(user_model,
                                                                   "foo-bar");

  // Assert
  EXPECT_FALSE(input_variable.does_match_child);
  EXPECT_FALSE(input_variable.does_match_parent);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorMatchingChildInterestSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.interest_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInterestSegmentInputVariable(user_model,
                                                             "parent-child");

  // Assert
  EXPECT_TRUE(input_variable.does_match_child);
  EXPECT_TRUE(input_variable.does_match_parent);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorMatchingParentInterestSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.interest_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInterestSegmentInputVariable(user_model,
                                                             "parent-foo");

  // Assert
  EXPECT_FALSE(input_variable.does_match_child);
  EXPECT_TRUE(input_variable.does_match_parent);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorNonMatchingInterestSegmentInputVariable) {
  // Arrange
  UserModelInfo user_model;
  user_model.interest_segments = {"parent-child", "xyzzy-thud"};

  // Act
  const CreativeAdPredictorSegmentInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInterestSegmentInputVariable(user_model,
                                                             "foo-bar");

  // Assert
  EXPECT_FALSE(input_variable.does_match_child);
  EXPECT_FALSE(input_variable.does_match_parent);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorLastSeenAdInputVariable) {
  // Arrange
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEventForTesting(creative_ad, AdType::kNotificationAd,
                             ConfirmationType::kViewed, Now() - base::Hours(7));
  ad_events.push_back(ad_event);

  // Act
  const absl::optional<base::TimeDelta> input_variable =
      ComputeCreativeAdPredictorLastSeenAdInputVariable(creative_ad, ad_events);

  // Assert
  EXPECT_EQ(base::Hours(7), input_variable);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorLastSeenAdInputVariableIfNeverSeen) {
  // Arrange
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);

  const AdEventList ad_events;

  // Act
  const absl::optional<base::TimeDelta> input_variable =
      ComputeCreativeAdPredictorLastSeenAdInputVariable(creative_ad, ad_events);

  // Assert
  EXPECT_FALSE(input_variable);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorLastSeenAdvertiserInputVariable) {
  // Arrange
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEventForTesting(creative_ad, AdType::kNotificationAd,
                             ConfirmationType::kViewed, Now() - base::Hours(3));
  ad_events.push_back(ad_event);

  // Act
  const absl::optional<base::TimeDelta> input_variable =
      ComputeCreativeAdPredictorLastSeenAdvertiserInputVariable(creative_ad,
                                                                ad_events);

  // Assert
  EXPECT_EQ(base::Hours(3), input_variable);
}

TEST_F(HnsAdsCreativeAdPredictorInputVariableUtilTest,
       ComputeCreativeAdPredictorLastSeenAdvertiserInputVariableIfNeverSeen) {
  // Arrange
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);

  const AdEventList ad_events;

  // Act
  const absl::optional<base::TimeDelta> input_variable =
      ComputeCreativeAdPredictorLastSeenAdvertiserInputVariable(creative_ad,
                                                                ad_events);

  // Assert
  EXPECT_FALSE(input_variable);
}

}  // namespace hns_ads
