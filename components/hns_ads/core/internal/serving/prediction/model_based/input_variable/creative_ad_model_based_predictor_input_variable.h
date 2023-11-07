/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_H_

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_input_variable_info.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_input_variable_util.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_segment_input_variable_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace hns_ads {

struct UserModelInfo;

template <typename T>
CreativeAdPredictorInputVariableInfo ComputeCreativeAdPredictorInputVariable(
    const T& creative_ad,
    const UserModelInfo& user_model,
    const AdEventList& ad_events) {
  CreativeAdPredictorInputVariableInfo input_variable;

  input_variable.intent_segment =
      ComputeCreativeAdPredictorIntentSegmentInputVariable(user_model,
                                                           creative_ad.segment);

  input_variable.latent_interest_segment =
      ComputeCreativeAdPredictorLatentInterestSegmentInputVariable(
          user_model, creative_ad.segment);

  input_variable.interest_segment =
      ComputeCreativeAdPredictorInterestSegmentInputVariable(
          user_model, creative_ad.segment);

  input_variable.last_seen_ad =
      ComputeCreativeAdPredictorLastSeenAdInputVariable(creative_ad, ad_events);

  input_variable.last_seen_advertiser =
      ComputeCreativeAdPredictorLastSeenAdvertiserInputVariable(creative_ad,
                                                                ad_events);

  return input_variable;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_H_
