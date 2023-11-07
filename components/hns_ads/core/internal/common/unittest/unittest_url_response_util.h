/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_URL_RESPONSE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_URL_RESPONSE_UTIL_H_

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_url_response_alias.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

absl::optional<mojom::UrlResponseInfo> GetNextUrlResponseForRequest(
    const mojom::UrlRequestInfoPtr& url_request,
    const URLResponseMap& url_responses);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_URL_RESPONSE_UTIL_H_
