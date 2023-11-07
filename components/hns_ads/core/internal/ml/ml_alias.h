/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_ML_ALIAS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_ML_ALIAS_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/ml/transformation/transformation.h"

namespace hns_ads::ml {

using PredictionMap = std::map</*class*/ std::string, /*prediction*/ double>;
using TransformationPtr = std::unique_ptr<Transformation>;
using TransformationVector = std::vector<TransformationPtr>;

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_ML_ALIAS_H_
