/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_WIREGUARD_TUNNEL_SERVICE_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_WIREGUARD_TUNNEL_SERVICE_H_

#include <string>

namespace base {
class FilePath;
}  // namespace base

namespace hns_vpn {

namespace wireguard {

// Functions used from HnsWireguardManager to create and launch a new service.
bool LaunchWireguardService(const std::wstring& config);
bool RemoveExistingWireguardService();
bool CreateAndRunHnsWireguardService(const std::wstring& config);

// Functions used inside Wireguard service to setup connection.
int RunWireguardTunnelService(const base::FilePath& config);
bool WireguardGenerateKeypair(std::string* public_key,
                              std::string* private_key);
}  // namespace wireguard

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_WIREGUARD_TUNNEL_SERVICE_H_
