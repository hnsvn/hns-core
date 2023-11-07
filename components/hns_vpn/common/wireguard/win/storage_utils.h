/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_STORAGE_UTILS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_STORAGE_UTILS_H_

#include "base/files/file_path.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_vpn {

namespace wireguard {
std::wstring GetHnsVpnWireguardServiceRegistryStoragePath();
absl::optional<base::FilePath> GetLastUsedConfigPath();
bool UpdateLastUsedConfigPath(const base::FilePath& config_path);
void RemoveStorageKey();
}  // namespace wireguard

bool IsVPNTrayIconEnabled();
void EnableVPNTrayIcon(bool value);

void SetWireguardActive(bool value);
bool IsWireguardActive();

bool ShouldFallbackToIKEv2();
void IncrementWireguardTunnelUsageFlag();
void ResetWireguardTunnelUsageFlag();

void WriteConnectionState(int state);
absl::optional<int32_t> GetConnectionState();

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_STORAGE_UTILS_H_
