/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_VERIFIABLE_CONVERSION_ID_PATTERN_VERIFIABLE_CONVERSION_ID_PATTERN_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_VERIFIABLE_CONVERSION_ID_PATTERN_VERIFIABLE_CONVERSION_ID_PATTERN_UTIL_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_id_pattern_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class GURL;

namespace hns_ads {

absl::optional<std::string> MaybeParseVerifiableConversionId(
    const std::vector<GURL>& redirect_chain,
    const std::string& html,
    const ConversionResourceIdPatternMap& resource_id_patterns);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_VERIFIABLE_CONVERSION_ID_PATTERN_VERIFIABLE_CONVERSION_ID_PATTERN_UTIL_H_
