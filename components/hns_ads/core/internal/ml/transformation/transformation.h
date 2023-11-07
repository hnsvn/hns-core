/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_TRANSFORMATION_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_TRANSFORMATION_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/ml/transformation/transformation_types.h"

namespace hns_ads::ml {

class Data;

class Transformation {
 public:
  explicit Transformation(const TransformationType& type);

  virtual ~Transformation();

  TransformationType GetType() const;

  virtual std::unique_ptr<Data> Apply(
      const std::unique_ptr<Data>& input_data) const = 0;

 protected:
  const TransformationType type_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_TRANSFORMATION_H_
