/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_WIREGUARD_UTILS_WIN_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_WIREGUARD_UTILS_WIN_H_

#include <string>
#include <tuple>

#include "base/functional/callback.h"
#include "hns/components/hns_vpn/common/wireguard/wireguard_utils.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_vpn {

namespace wireguard {

bool IsHnsVPNWireguardTunnelServiceRunning();
bool IsWireguardServiceRegistered();
void WireguardGenerateKeypair(WireguardGenerateKeypairCallback callback);
absl::optional<std::string> CreateWireguardConfig(
    const std::string& client_private_key,
    const std::string& server_public_key,
    const std::string& vpn_server_hostname,
    const std::string& mapped_ipv4_address,
    const std::string& dns_servers);
void EnableHnsVpnWireguardService(const std::string& config,
                                    BooleanCallback callback);
void DisableHnsVpnWireguardService(BooleanCallback callback);

void SetWireguardServiceRegisteredForTesting(bool value);
void ShowHnsVpnStatusTrayIcon();

}  // namespace wireguard

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_WIREGUARD_UTILS_WIN_H_
