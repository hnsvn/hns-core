/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_UTIL_H_

#include <string>

#include "third_party/abseil-cpp/absl/types/optional.h"

class GURL;

namespace hns_ads {

absl::optional<std::string> GetPlacementIdFromSearchResultAdClickedUrl(
    const GURL& url);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_UTIL_H_
