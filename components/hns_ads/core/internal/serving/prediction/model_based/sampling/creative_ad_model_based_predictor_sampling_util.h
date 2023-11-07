/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_SAMPLING_CREATIVE_AD_MODEL_BASED_PREDICTOR_SAMPLING_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_SAMPLING_CREATIVE_AD_MODEL_BASED_PREDICTOR_SAMPLING_UTIL_H_

#include <numeric>

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor_info.h"

namespace hns_ads {

template <typename T>
double CalculateNormalizingConstantForCreativeAdPredictors(
    const CreativeAdPredictorList<T>& creative_ad_predictors) {
  return std::accumulate(
      creative_ad_predictors.cbegin(), creative_ad_predictors.cend(), 0.0,
      [](double normalizing_constant,
         const CreativeAdPredictorList<T>::value_type& creative_ad_predictor) {
        return normalizing_constant + creative_ad_predictor.score;
      });
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_MODEL_BASED_SAMPLING_CREATIVE_AD_MODEL_BASED_PREDICTOR_SAMPLING_UTIL_H_
