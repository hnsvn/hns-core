/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_SEGMENT_INPUT_VARIABLE_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_SEGMENT_INPUT_VARIABLE_INFO_H_

namespace hns_ads {

struct CreativeAdPredictorSegmentInputVariableInfo final {
  bool does_match_child = false;
  bool does_match_parent = false;
};

bool operator==(const CreativeAdPredictorSegmentInputVariableInfo&,
                const CreativeAdPredictorSegmentInputVariableInfo&);
bool operator!=(const CreativeAdPredictorSegmentInputVariableInfo&,
                const CreativeAdPredictorSegmentInputVariableInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_SEGMENT_INPUT_VARIABLE_INFO_H_
