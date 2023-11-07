/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/model/linear/linear.h"

#include <utility>
#include <vector>

#include "base/containers/adapters.h"
#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/ml/ml_prediction_util.h"

namespace hns_ads::ml {

LinearModel::LinearModel() = default;

LinearModel::LinearModel(std::map<std::string, VectorData> weights,
                         std::map<std::string, double> biases) {
  weights_ = std::move(weights);
  biases_ = std::move(biases);
}

LinearModel::LinearModel(const LinearModel& other) = default;

LinearModel& LinearModel::operator=(const LinearModel& other) = default;

LinearModel::LinearModel(LinearModel&& other) noexcept = default;

LinearModel& LinearModel::operator=(LinearModel&& other) noexcept = default;

LinearModel::~LinearModel() = default;

PredictionMap LinearModel::Predict(const VectorData& data) const {
  PredictionMap predictions;
  for (const auto& kv : weights_) {
    double prediction = kv.second * data;
    const auto iter = biases_.find(kv.first);
    if (iter != biases_.cend()) {
      prediction += iter->second;
    }
    predictions[kv.first] = prediction;
  }
  return predictions;
}

PredictionMap LinearModel::GetTopPredictions(const VectorData& data) const {
  return GetTopCountPredictionsImpl(data, absl::nullopt);
}

PredictionMap LinearModel::GetTopCountPredictions(const VectorData& data,
                                                  size_t top_count) const {
  return GetTopCountPredictionsImpl(data, top_count);
}

PredictionMap LinearModel::GetTopCountPredictionsImpl(
    const VectorData& data,
    absl::optional<size_t> top_count) const {
  const PredictionMap prediction_map = Predict(data);
  const PredictionMap prediction_map_softmax = Softmax(prediction_map);
  std::vector<std::pair<double, std::string>> prediction_order;
  prediction_order.reserve(prediction_map_softmax.size());
  for (const auto& prediction : prediction_map_softmax) {
    prediction_order.emplace_back(prediction.second, prediction.first);
  }
  base::ranges::sort(base::Reversed(prediction_order));
  PredictionMap top_predictions;
  if (top_count && *top_count < prediction_order.size()) {
    prediction_order.resize(*top_count);
  }
  for (const auto& prediction_order_item : prediction_order) {
    top_predictions[prediction_order_item.second] = prediction_order_item.first;
  }
  return top_predictions;
}

}  // namespace hns_ads::ml
