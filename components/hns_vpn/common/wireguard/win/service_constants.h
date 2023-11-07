/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_CONSTANTS_H_

namespace hns_vpn {

constexpr wchar_t kHnsVpnWireguardServiceExecutable[] =
    L"hns_vpn_wireguard_service.exe";

// Registry flag to count service launches for the fallback.
constexpr wchar_t kHnsVpnWireguardCounterOfTunnelUsage[] =
    L"tunnel_launches_counter";

// Register and configure windows service.
constexpr char kHnsVpnWireguardServiceInstallSwitchName[] = "install";

// Remove config and all stuff related to service.
constexpr char kHnsVpnWireguardServiceUnnstallSwitchName[] = "uninstall";

// Load wireguard binaries and connect to vpn using passed config.
constexpr char kHnsVpnWireguardServiceConnectSwitchName[] = "connect";

// In this mode the service started on user level and expose UI interfaces
// to work with the service for a user.
constexpr char kHnsVpnWireguardServiceInteractiveSwitchName[] = "interactive";

// Notifies users about connected state of the vpn using system notifications.
constexpr char kHnsVpnWireguardServiceNotifyConnectedSwitchName[] =
    "notify-connected";

// Notifies users about disconnected state of the vpn using system
// notifications.
constexpr char kHnsVpnWireguardServiceNotifyDisconnectedSwitchName[] =
    "notify-disconnected";
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_CONSTANTS_H_
