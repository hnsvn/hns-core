/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/transformation/transformation.h"

namespace hns_ads::ml {

Transformation::Transformation(const TransformationType& type) : type_(type) {}

Transformation::~Transformation() = default;

TransformationType Transformation::GetType() const {
  return type_;
}

}  // namespace hns_ads::ml
