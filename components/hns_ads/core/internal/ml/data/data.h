/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_DATA_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_DATA_H_

#include "hns/components/hns_ads/core/internal/ml/data/data_types.h"

namespace hns_ads::ml {

class Data {
 public:
  explicit Data(const DataType& type);

  Data(const Data&) = delete;
  Data& operator=(const Data&) = delete;

  Data(Data&&) noexcept = delete;
  Data& operator=(Data&&) noexcept = delete;

  virtual ~Data();

  DataType GetType() const;

 protected:
  const DataType type_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_DATA_H_
