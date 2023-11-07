/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/data/text_data.h"

#include <utility>

namespace hns_ads::ml {

TextData::TextData() : Data(DataType::kText) {}

TextData::TextData(std::string text)
    : Data(DataType::kText), text_(std::move(text)) {}

const std::string& TextData::GetText() const {
  return text_;
}

}  // namespace hns_ads::ml
