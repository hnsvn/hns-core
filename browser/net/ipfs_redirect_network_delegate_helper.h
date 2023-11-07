/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_IPFS_REDIRECT_NETWORK_DELEGATE_HELPER_H_
#define HNS_BROWSER_NET_IPFS_REDIRECT_NETWORK_DELEGATE_HELPER_H_

#include <memory>
#include "hns/browser/net/url_context.h"
#include "net/base/completion_once_callback.h"
#include "net/http/http_response_headers.h"
#include "url/gurl.h"

namespace ipfs {

int OnBeforeURLRequest_IPFSRedirectWork(
    const hns::ResponseCallback& next_callback,
    std::shared_ptr<hns::HnsRequestInfo> ctx);

}  // namespace ipfs

#endif  // HNS_BROWSER_NET_IPFS_REDIRECT_NETWORK_DELEGATE_HELPER_H_
