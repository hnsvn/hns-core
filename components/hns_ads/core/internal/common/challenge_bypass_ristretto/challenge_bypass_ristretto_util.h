/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_CHALLENGE_BYPASS_RISTRETTO_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_CHALLENGE_BYPASS_RISTRETTO_UTIL_H_

#include <string>
#include <utility>

#include "base/types/expected.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::cbr {

template <typename T, typename U = T>
absl::optional<U> ValueOrLogError(base::expected<T, std::string> result) {
  if (!result.has_value()) {
    BLOG(1, "Challenge Bypass Ristretto Error: " << result.error());
    return absl::nullopt;
  }
  return absl::make_optional<U>(std::move(result).value());
}

}  // namespace hns_ads::cbr

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_CHALLENGE_BYPASS_RISTRETTO_UTIL_H_
