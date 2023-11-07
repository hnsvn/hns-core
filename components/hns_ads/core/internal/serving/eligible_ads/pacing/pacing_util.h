/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PACING_PACING_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PACING_PACING_UTIL_H_

#include <ios>

#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pacing/pacing_random_util.h"

namespace hns_ads {

template <typename T>
bool ShouldPaceAd(const T& ad) {
  const double rand = GeneratePacingRandomNumber();
  if (rand < ad.pass_through_rate) {
    return false;
  }

  BLOG(2, std::fixed << "Pacing delivery for creative instance id "
                     << ad.creative_instance_id << " [Roll("
                     << ad.pass_through_rate << "):" << rand << "]");

  return true;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PACING_PACING_UTIL_H_
