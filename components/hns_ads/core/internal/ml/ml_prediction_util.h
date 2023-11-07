/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_ML_PREDICTION_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_ML_PREDICTION_UTIL_H_

#include "hns/components/hns_ads/core/internal/ml/ml_alias.h"

namespace hns_ads::ml {

PredictionMap Softmax(const PredictionMap& predictions);

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_ML_PREDICTION_UTIL_H_
