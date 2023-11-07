/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_SCORING_CREATIVE_AD_MODEL_BASED_PREDICTOR_SCORING_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_SCORING_CREATIVE_AD_MODEL_BASED_PREDICTOR_SCORING_H_

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_input_variable_info.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/scoring/creative_ad_model_based_predictor_scoring_util.h"

namespace hns_ads {

template <typename T>
double ComputeCreativeAdPredictorScore(
    const T& creative_ad,
    const CreativeAdPredictorInputVariableInfo& input_variable) {
  double score = 0.0;

  score += ComputeIntentSegmentScore(input_variable.intent_segment);

  score +=
      ComputeLatentInterestSegmentScore(input_variable.latent_interest_segment);

  score += ComputeInterestSegmentScore(input_variable.interest_segment);

  score += ComputeLastSeenAdScore(input_variable.last_seen_ad);

  score += ComputeLastSeenAdvertiserScore(input_variable.last_seen_advertiser);

  score += ComputePriorityScore(creative_ad.priority);

  return score;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_SCORING_CREATIVE_AD_MODEL_BASED_PREDICTOR_SCORING_H_
