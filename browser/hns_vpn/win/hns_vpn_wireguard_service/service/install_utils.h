/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_INSTALL_UTILS_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_INSTALL_UTILS_H_

#include <string>

namespace hns_vpn {

bool ConfigureHnsWireguardService(const std::wstring& service_name);
bool InstallHnsWireguardService();
bool UninstallHnsWireguardService();

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_INSTALL_UTILS_H_
