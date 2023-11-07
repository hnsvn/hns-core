/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_HNS_HTTPSE_NETWORK_DELEGATE_H_
#define HNS_BROWSER_NET_HNS_HTTPSE_NETWORK_DELEGATE_H_

#include "hns/browser/net/url_context.h"

namespace hns {

int OnBeforeURLRequest_HttpsePreFileWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx);

}  // namespace hns

#endif  // HNS_BROWSER_NET_HNS_NETWORK_DELEGATE_H_
