/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_ICON_UTILS_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_ICON_UTILS_H_

namespace gfx {
class ImageSkia;
class Size;
}  // namespace gfx

namespace hns_vpn {
gfx::ImageSkia GetIconFromResources(int icon_id, const gfx::Size& size);
}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_ICON_UTILS_H_
