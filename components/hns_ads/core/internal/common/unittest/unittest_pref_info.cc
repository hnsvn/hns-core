/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_info.h"

namespace hns_ads {

PrefInfo::PrefInfo() = default;

PrefInfo::PrefInfo(const PrefInfo& other) = default;
PrefInfo& PrefInfo::operator=(const PrefInfo& other) = default;

PrefInfo::PrefInfo(PrefInfo&& other) noexcept = default;
PrefInfo& PrefInfo::operator=(PrefInfo&& other) noexcept = default;

PrefInfo::~PrefInfo() = default;

bool operator==(const PrefInfo& lhs, const PrefInfo& rhs) {
  const auto tie = [](const PrefInfo& pref) {
    return std::tie(pref.value, pref.default_value);
  };

  return tie(lhs) == tie(rhs);
}

bool operator!=(const PrefInfo& lhs, const PrefInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace hns_ads
