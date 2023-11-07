/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_VPN_UTILS_H_
#define HNS_BROWSER_HNS_VPN_VPN_UTILS_H_

namespace content {
class BrowserContext;
}  // namespace content

namespace hns_vpn {

bool IsHnsVPNEnabled(content::BrowserContext* context);
bool IsAllowedForContext(content::BrowserContext* context);

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_VPN_UTILS_H_
