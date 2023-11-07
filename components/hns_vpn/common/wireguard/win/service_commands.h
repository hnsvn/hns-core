/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_COMMANDS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_COMMANDS_H_

#include <string>

namespace hns_vpn {
void RunWireGuardCommandForUsers(const std::string& command);
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_COMMANDS_H_
