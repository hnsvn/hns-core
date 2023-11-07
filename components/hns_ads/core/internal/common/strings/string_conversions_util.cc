/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/strings/string_conversions_util.h"

#include "base/strings/string_split.h"

namespace hns_ads {

namespace {

constexpr char kTrue[] = "true";
constexpr char kFalse[] = "false";

}  // namespace

std::string BoolToString(const bool value) {
  return value ? kTrue : kFalse;
}

std::vector<float> DelimitedStringToVector(const std::string& string,
                                           const base::StringPiece delimiter) {
  const std::vector<std::string> string_components = base::SplitString(
      string, delimiter, base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);

  std::vector<float> vector_components;
  vector_components.reserve(string_components.size());
  for (const auto& string_component : string_components) {
    double value;
    base::StringToDouble(string_component, &value);
    vector_components.push_back(static_cast<float>(value));
  }

  return vector_components;
}

}  // namespace hns_ads