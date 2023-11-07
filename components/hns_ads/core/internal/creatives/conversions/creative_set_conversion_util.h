/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_UTIL_H_

#include <map>
#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class GURL;

namespace hns_ads {

using CreativeSetConversionBuckets =
    std::map</*creative_set_id*/ std::string, CreativeSetConversionList>;

CreativeSetConversionList FilterConvertedAndNonMatchingCreativeSetConversions(
    const CreativeSetConversionList& creative_set_conversions,
    const AdEventList& ad_events,
    const std::vector<GURL>& redirect_chain);

CreativeSetConversionBuckets SortCreativeSetConversionsIntoBuckets(
    const CreativeSetConversionList& creative_set_conversions);

absl::optional<CreativeSetConversionInfo> FindNonExpiredCreativeSetConversion(
    const CreativeSetConversionList& creative_set_conversions,
    const AdEventInfo& ad_event);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_UTIL_H_
