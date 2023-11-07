/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ALLOCATION_SEEN_ADS_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ALLOCATION_SEEN_ADS_UTIL_H_

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

struct CreativeAdInfo;

absl::optional<base::Time> GetLastSeenAdAt(const AdEventList& ad_events,
                                           const CreativeAdInfo& creative_ad);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ALLOCATION_SEEN_ADS_UTIL_H_
