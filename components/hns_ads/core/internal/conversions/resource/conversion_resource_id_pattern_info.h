/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_ID_PATTERN_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_ID_PATTERN_INFO_H_

#include <map>
#include <string>

#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_id_pattern_search_in_types.h"

namespace hns_ads {

struct ConversionResourceIdPatternInfo final {
  std::string url_pattern;
  ConversionResourceIdPatternSearchInType search_in_type =
      ConversionResourceIdPatternSearchInType::kDefault;
  std::string id_pattern;
};

bool operator==(const ConversionResourceIdPatternInfo&,
                const ConversionResourceIdPatternInfo&);
bool operator!=(const ConversionResourceIdPatternInfo&,
                const ConversionResourceIdPatternInfo&);

using ConversionResourceIdPatternMap =
    std::map</*resource_url_pattern*/ std::string,
             ConversionResourceIdPatternInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_ID_PATTERN_INFO_H_
