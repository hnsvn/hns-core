/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_HNS_REDUCE_LANGUAGE_NETWORK_DELEGATE_HELPER_H_
#define HNS_BROWSER_NET_HNS_REDUCE_LANGUAGE_NETWORK_DELEGATE_HELPER_H_

#include <memory>
#include <string>

#include "hns/browser/net/url_context.h"
#include "url/gurl.h"

class HostContentSettingsMap;
class PrefService;
class Profile;
struct HnsRequestInfo;

namespace net {
class HttpRequestHeaders;
class URLRequest;
}  // namespace net

namespace hns {

std::string FarbleAcceptLanguageHeader(
    const GURL& tab_origin,
    Profile* profile,
    HostContentSettingsMap* content_settings);

int OnBeforeStartTransaction_ReduceLanguageWork(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx);

}  // namespace hns

#endif  // HNS_BROWSER_NET_HNS_REDUCE_LANGUAGE_NETWORK_DELEGATE_HELPER_H_
