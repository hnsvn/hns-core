/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_TEXT_DATA_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_TEXT_DATA_H_

#include <string>

#include "hns/components/hns_ads/core/internal/ml/data/data.h"

namespace hns_ads::ml {

class TextData final : public Data {
 public:
  TextData();
  explicit TextData(std::string text);

  const std::string& GetText() const;

 private:
  std::string text_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_DATA_TEXT_DATA_H_
