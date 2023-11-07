/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_URL_URL_RESPONSE_STRING_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_URL_URL_RESPONSE_STRING_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

std::string UrlResponseToString(const mojom::UrlResponseInfo& url_response);
std::string UrlResponseHeadersToString(
    const mojom::UrlResponseInfo& url_response);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_URL_URL_RESPONSE_STRING_UTIL_H_
