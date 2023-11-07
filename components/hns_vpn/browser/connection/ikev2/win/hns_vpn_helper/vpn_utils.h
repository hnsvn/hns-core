// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_VPN_UTILS_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_VPN_UTILS_H_

#include <windows.h>
#include <string>

namespace hns_vpn {
// Sets helper's flag to indicate filters successfully installed.
void SetFiltersInstalledFlag();
// Resets helper's filters installed flag.
void ResetFiltersInstalledFlag();
// Register and setup DNS filters layer to the system, if the layer is already
// registered reuses existing.
bool AddWpmFilters(HANDLE engine_handle, const std::string& name);
// Opens a session to a filter engine.
HANDLE OpenWpmSession();
// Closes a session to a filter engine.
bool CloseWpmSession(HANDLE engine);
// Subscribes for RAS connection notification of any os vpn entry.
bool SubscribeRasConnectionNotification(HANDLE event_handle);
// Configure VPN Service autorestart.
bool ConfigureServiceAutoRestart(const std::wstring& service_name,
                                 const std::wstring& hns_vpn_entry);
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_VPN_UTILS_H_
