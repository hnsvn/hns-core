/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/data/data.h"

namespace hns_ads::ml {

Data::Data(const DataType& type) : type_(type) {}

Data::~Data() = default;

DataType Data::GetType() const {
  return type_;
}

}  // namespace hns_ads::ml
