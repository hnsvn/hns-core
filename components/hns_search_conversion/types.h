/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SEARCH_CONVERSION_TYPES_H_
#define HNS_COMPONENTS_HNS_SEARCH_CONVERSION_TYPES_H_

namespace hns_search_conversion {

enum class ConversionType {
  kNone = 0,
  kButton,
  kNTP,
  kBannerTypeA,
  kBannerTypeB,
  kBannerTypeC,
  kBannerTypeD,
};

}  // namespace hns_search_conversion

#endif  // HNS_COMPONENTS_HNS_SEARCH_CONVERSION_TYPES_H_
