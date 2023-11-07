/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_INFO_H_

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_segment_input_variable_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct CreativeAdPredictorInputVariableInfo final {
  CreativeAdPredictorSegmentInputVariableInfo intent_segment;
  CreativeAdPredictorSegmentInputVariableInfo latent_interest_segment;
  CreativeAdPredictorSegmentInputVariableInfo interest_segment;
  absl::optional<base::TimeDelta> last_seen_ad;
  absl::optional<base::TimeDelta> last_seen_advertiser;
};

bool operator==(const CreativeAdPredictorInputVariableInfo&,
                const CreativeAdPredictorInputVariableInfo&);
bool operator!=(const CreativeAdPredictorInputVariableInfo&,
                const CreativeAdPredictorInputVariableInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_INFO_H_
