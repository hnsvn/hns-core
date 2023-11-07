/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_DEFAULT_CONVERSION_CREATIVE_SET_CONVERSION_URL_PATTERN_CREATIVE_SET_CONVERSION_URL_PATTERN_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_DEFAULT_CONVERSION_CREATIVE_SET_CONVERSION_URL_PATTERN_CREATIVE_SET_CONVERSION_URL_PATTERN_UTIL_H_

#include <vector>

class GURL;

namespace hns_ads {

struct CreativeSetConversionInfo;

bool DoesCreativeSetConversionUrlPatternMatchRedirectChain(
    const CreativeSetConversionInfo& creative_set_conversion,
    const std::vector<GURL>& redirect_chain);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_DEFAULT_CONVERSION_CREATIVE_SET_CONVERSION_URL_PATTERN_CREATIVE_SET_CONVERSION_URL_PATTERN_UTIL_H_
