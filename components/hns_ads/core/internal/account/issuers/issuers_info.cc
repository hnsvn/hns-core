/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_info.h"

#include <tuple>

namespace hns_ads {

IssuersInfo::IssuersInfo() = default;

IssuersInfo::IssuersInfo(const IssuersInfo& other) = default;

IssuersInfo& IssuersInfo::operator=(const IssuersInfo& other) = default;

IssuersInfo::IssuersInfo(IssuersInfo&& other) noexcept = default;

IssuersInfo& IssuersInfo::operator=(IssuersInfo&& other) noexcept = default;

IssuersInfo::~IssuersInfo() = default;

bool IssuersInfo::operator==(const IssuersInfo& other) const {
  const auto tie = [](const IssuersInfo& info) {
    return std::tie(info.ping, info.issuers);
  };

  return tie(*this) == tie(other);
}

bool IssuersInfo::operator!=(const IssuersInfo& other) const {
  return !(*this == other);
}

}  // namespace hns_ads
