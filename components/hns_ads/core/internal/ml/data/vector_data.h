/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_VECTOR_DATA_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_VECTOR_DATA_H_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "hns/components/hns_ads/core/internal/ml/data/data.h"

namespace hns_ads::ml {

class VectorData final : public Data {
 public:
  VectorData();

  // Make a "dense" DataVector with points 0..n-1 (n = data.size()):
  // ({0, data[0]}, {1, data[0]}, .., {n-1, data[n-1]}}
  explicit VectorData(std::vector<float> data);

  // Make a "sparse" DataVector using points from |data|.
  // double is used for backward compatibility with the current code.
  VectorData(size_t dimension_count, const std::map<uint32_t, double>& data);

  // Explicit copy assignment && move operators is required because the class
  // inherits const member type_ that cannot be copied by default
  VectorData(const VectorData& vector_data);
  VectorData& operator=(const VectorData& vector_data);

  // Explicit copy assignment && move operators is required because the class
  // inherits const member type_ that cannot be copied by default
  VectorData(VectorData&& vector_data) noexcept;
  VectorData& operator=(VectorData&& vector_data) noexcept;

  ~VectorData() override;

  // Mathematical vector operations
  friend float operator*(const VectorData&, const VectorData&);
  float ComputeSimilarity(const VectorData& other) const;

  void AddElementWise(const VectorData& other);
  void DivideByScalar(float scalar);
  void Normalize();

  bool IsEmpty() const;
  size_t GetDimensionCount() const;
  size_t GetNonZeroElementCount() const;
  float GetNorm() const;

  const std::vector<float>& GetData() const;

 private:
  std::unique_ptr<class VectorDataStorage> storage_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_VECTOR_DATA_H_
