/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/allocation/seen_ads_util.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/allocation/seen_advertisers_util.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_segment_input_variable_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct UserModelInfo;

CreativeAdPredictorSegmentInputVariableInfo
ComputeCreativeAdPredictorIntentSegmentInputVariable(
    const UserModelInfo& user_model,
    const std::string& segment);

CreativeAdPredictorSegmentInputVariableInfo
ComputeCreativeAdPredictorLatentInterestSegmentInputVariable(
    const UserModelInfo& user_model,
    const std::string& segment);

CreativeAdPredictorSegmentInputVariableInfo
ComputeCreativeAdPredictorInterestSegmentInputVariable(
    const UserModelInfo& user_model,
    const std::string& segment);

template <typename T>
absl::optional<base::TimeDelta>
ComputeCreativeAdPredictorLastSeenAdInputVariable(
    const T& creative_ad,
    const AdEventList& ad_events) {
  const absl::optional<base::Time> last_seen_at =
      GetLastSeenAdAt(ad_events, creative_ad);
  if (!last_seen_at) {
    return absl::nullopt;
  }

  return base::Time::Now() - *last_seen_at;
}

template <typename T>
absl::optional<base::TimeDelta>
ComputeCreativeAdPredictorLastSeenAdvertiserInputVariable(
    const T& creative_ad,
    const AdEventList& ad_events) {
  const absl::optional<base::Time> last_seen_at =
      GetLastSeenAdvertiserAt(ad_events, creative_ad);
  if (!last_seen_at) {
    return absl::nullopt;
  }

  return base::Time::Now() - *last_seen_at;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_INPUT_VARIABLE_CREATIVE_AD_MODEL_BASED_PREDICTOR_INPUT_VARIABLE_UTIL_H_
