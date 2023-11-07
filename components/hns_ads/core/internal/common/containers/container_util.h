/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CONTAINERS_CONTAINER_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CONTAINERS_CONTAINER_UTIL_H_

#include <iterator>
#include <vector>

#include "base/check_op.h"

namespace hns_ads {

template <typename T>
std::vector<std::vector<T>> SplitVector(const std::vector<T>& elements,
                                        const int chunk_size) {
  CHECK_NE(0, chunk_size);

  std::vector<std::vector<T>> result;
  result.reserve((elements.size() + chunk_size - 1) / chunk_size);

  auto begin = elements.cbegin();
  const auto end = elements.cend();
  while (begin != end) {
    const auto next =
        std::distance(begin, end) >= chunk_size ? begin + chunk_size : end;

    result.emplace_back(begin, next);

    begin = next;
  }

  return result;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CONTAINERS_CONTAINER_UTIL_H_
