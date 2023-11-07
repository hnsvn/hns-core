/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_HELPER_STATE_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_HELPER_STATE_H_

#include <string>

namespace base {
class FilePath;
}  // namespace base

namespace hns_vpn {

bool IsHnsVPNHelperServiceInstalled();
bool IsNetworkFiltersInstalled();
std::wstring GetHnsVPNConnectionName();
std::wstring GetHnsVpnHelperServiceName();
std::wstring GetHnsVpnHelperServiceDisplayName();
base::FilePath GetVpnHelperServiceProfileDir();
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_HELPER_STATE_H_
