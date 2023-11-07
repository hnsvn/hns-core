/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_MODEL_LINEAR_LINEAR_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_MODEL_LINEAR_LINEAR_H_

#include <map>
#include <string>

#include "hns/components/hns_ads/core/internal/ml/data/vector_data.h"
#include "hns/components/hns_ads/core/internal/ml/ml_alias.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::ml {

class LinearModel final {
 public:
  LinearModel();

  explicit LinearModel(const std::string& model);
  LinearModel(std::map<std::string, VectorData> weights,
              std::map<std::string, double> biases);

  LinearModel(const LinearModel&);
  LinearModel& operator=(const LinearModel&);

  LinearModel(LinearModel&&) noexcept;
  LinearModel& operator=(LinearModel&&) noexcept;

  ~LinearModel();

  PredictionMap Predict(const VectorData& data) const;

  PredictionMap GetTopPredictions(const VectorData& data) const;

  PredictionMap GetTopCountPredictions(const VectorData& data,
                                       size_t top_count) const;

 private:
  PredictionMap GetTopCountPredictionsImpl(
      const VectorData& data,
      absl::optional<size_t> top_count) const;

  std::map<std::string, VectorData> weights_;
  std::map<std::string, double> biases_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_MODEL_LINEAR_LINEAR_H_
