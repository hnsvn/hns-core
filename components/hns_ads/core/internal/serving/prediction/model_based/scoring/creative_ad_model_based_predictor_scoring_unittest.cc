/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/scoring/creative_ad_model_based_predictor_scoring.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor_feature.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_input_variable.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeAdPredictorScoringTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeAdPredictorScoringTest,
       ComputeCreativeAdPredictorScoreForDefaultWeights) {
  // Arrange
  CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "parent-child";

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"parent-child"},
      /*latent_interest_segments*/ {"parent-child"},
      /*interest_segments*/ {"parent-child"},
      /*text_embedding_html_events*/ {});

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEventForTesting(creative_ad, AdType::kNotificationAd,
                             ConfirmationType::kViewed, Now() - base::Hours(7));
  ad_events.push_back(ad_event);

  const CreativeAdPredictorInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInputVariable(creative_ad, user_model,
                                              ad_events);

  // Act
  const double score =
      ComputeCreativeAdPredictorScore(creative_ad, input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(4.083333333333333, score);
}

TEST_F(HnsAdsCreativeAdPredictorScoringTest,
       ComputeCreativeAdPredictorScoreForNonDefaultWeights) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["child_intent_segment_ad_predictor_weight"] = "0.9";
  params["parent_intent_segment_ad_predictor_weight"] = "0.8";
  params["child_latent_interest_segment_ad_predictor_weight"] = "0.7";
  params["parent_latent_interest_segment_ad_predictor_weight"] = "0.6";
  params["child_interest_segment_ad_predictor_weight"] = "0.5";
  params["parent_interest_segment_ad_predictor_weight"] = "0.4";
  params["last_seen_ad_predictor_weight"] = "0.3";
  params["last_seen_advertiser_ad_predictor_weight"] = "0.2";
  params["priority_ad_predictor_weight"] = "0.1";
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "parent-child";

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"parent-child"},
      /*latent_interest_segments*/ {"parent-child"},
      /*interest_segments*/ {"parent-child"},
      /*text_embedding_html_events*/ {});

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEventForTesting(creative_ad, AdType::kNotificationAd,
                             ConfirmationType::kViewed, Now() - base::Hours(7));
  ad_events.push_back(ad_event);

  const CreativeAdPredictorInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInputVariable(creative_ad, user_model,
                                              ad_events);

  // Act
  const double score =
      ComputeCreativeAdPredictorScore(creative_ad, input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(2.2958333333333329, score);
}

TEST_F(HnsAdsCreativeAdPredictorScoringTest,
       ComputeCreativeAdPredictorScoreForZeroDefaultWeights) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["child_intent_segment_ad_predictor_weight"] = "0.0";
  params["parent_intent_segment_ad_predictor_weight"] = "0.0";
  params["child_latent_interest_segment_ad_predictor_weight"] = "0.0";
  params["parent_latent_interest_segment_ad_predictor_weight"] = "0.0";
  params["child_interest_segment_ad_predictor_weight"] = "0.0";
  params["parent_interest_segment_ad_predictor_weight"] = "0.0";
  params["last_seen_ad_predictor_weight"] = "0.0";
  params["last_seen_advertiser_ad_predictor_weight"] = "0.0";
  params["priority_ad_predictor_weight"] = "0.0";
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "parent-child";

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"parent-child"},
      /*latent_interest_segments*/ {"parent-child"},
      /*interest_segments*/ {"parent-child"},
      /*text_embedding_html_events*/ {});

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEventForTesting(creative_ad, AdType::kNotificationAd,
                             ConfirmationType::kViewed, Now() - base::Hours(7));
  ad_events.push_back(ad_event);

  const CreativeAdPredictorInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInputVariable(creative_ad, user_model,
                                              ad_events);

  // Act
  const double score =
      ComputeCreativeAdPredictorScore(creative_ad, input_variable);

  // Assert
  EXPECT_DOUBLE_EQ(0.0, score);
}

}  // namespace hns_ads
