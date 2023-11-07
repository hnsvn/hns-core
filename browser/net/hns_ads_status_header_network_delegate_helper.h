/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_HNS_ADS_STATUS_HEADER_NETWORK_DELEGATE_HELPER_H_
#define HNS_BROWSER_NET_HNS_ADS_STATUS_HEADER_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "hns/browser/net/url_context.h"

namespace net {
class HttpRequestHeaders;
}  // namespace net

namespace hns {

constexpr char kAdsStatusHeader[] = "X-Hns-Ads-Enabled";
constexpr char kAdsEnabledStatusValue[] = "1";

int OnBeforeStartTransaction_AdsStatusHeader(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx);

}  // namespace hns

#endif  // HNS_BROWSER_NET_HNS_ADS_STATUS_HEADER_NETWORK_DELEGATE_HELPER_H_
