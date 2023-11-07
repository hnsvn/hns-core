/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_ID_PATTERN_SEARCH_IN_TYPES_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_ID_PATTERN_SEARCH_IN_TYPES_H_

namespace hns_ads {

enum class ConversionResourceIdPatternSearchInType {
  kDefault = 0,  // kHtmlMetaTag
  kUrlRedirect,
  kHtml
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_ID_PATTERN_SEARCH_IN_TYPES_H_
