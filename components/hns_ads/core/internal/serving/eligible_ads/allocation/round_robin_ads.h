/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ALLOCATION_ROUND_ROBIN_ADS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ALLOCATION_ROUND_ROBIN_ADS_H_

#include <iterator>
#include <map>
#include <string>

#include "base/containers/contains.h"
#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"

namespace hns_ads {

template <typename T>
T FilterSeenAds(const T& ads, const std::map<std::string, bool>& seen_ads) {
  T unseen_ads;

  base::ranges::copy_if(ads, std::back_inserter(unseen_ads),
                        [&seen_ads](const CreativeAdInfo& creative_ad) {
                          return !base::Contains(
                              seen_ads, creative_ad.creative_instance_id);
                        });

  return unseen_ads;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ALLOCATION_ROUND_ROBIN_ADS_H_
