// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_NET_HNS_LOCALHOST_PERMISSION_NETWORK_DELEGATE_HELPER_H_
#define HNS_BROWSER_NET_HNS_LOCALHOST_PERMISSION_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "hns/browser/net/url_context.h"

struct HnsRequestInfo;

namespace hns {

int OnBeforeURLRequest_LocalhostPermissionWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx);

}  // namespace hns

#endif  // HNS_BROWSER_NET_HNS_LOCALHOST_PERMISSION_NETWORK_DELEGATE_HELPER_H_
