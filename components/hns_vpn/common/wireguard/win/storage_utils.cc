/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"

#include "base/logging.h"
#include "base/win/registry.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_constants.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_details.h"
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"

namespace hns_vpn {

namespace {
// Registry path to Wireguard vpn service storage.
constexpr wchar_t kHnsVpnWireguardServiceRegistryStoragePath[] =
    L"Software\\HnsSoftware\\Vpn\\";
constexpr wchar_t kHnsWireguardConfigKeyName[] = L"ConfigPath";
constexpr wchar_t kHnsWireguardEnableTrayIconKeyName[] = L"EnableTrayIcon";
constexpr wchar_t kHnsWireguardActiveKeyName[] = L"WireGuardActive";
constexpr wchar_t kHnsWireguardConnectionStateName[] = L"ConnectionState";
constexpr uint16_t kHnsVpnWireguardMaxFailedAttempts = 3;

absl::optional<base::win::RegKey> GetStorageKey(HKEY root_key, REGSAM access) {
  base::win::RegKey storage;
  if (storage.Create(
          root_key,
          hns_vpn::wireguard::GetHnsVpnWireguardServiceRegistryStoragePath()
              .c_str(),
          access) != ERROR_SUCCESS) {
    return absl::nullopt;
  }

  return storage;
}

}  // namespace

namespace wireguard {

std::wstring GetHnsVpnWireguardServiceRegistryStoragePath() {
  return kHnsVpnWireguardServiceRegistryStoragePath +
         hns_vpn::GetHnsVpnWireguardServiceName();
}

// Returns last used config path.
// We keep config file between launches to be able to reuse it outside of Hns.
absl::optional<base::FilePath> GetLastUsedConfigPath() {
  auto storage = GetStorageKey(HKEY_LOCAL_MACHINE, KEY_QUERY_VALUE);
  if (!storage.has_value()) {
    return absl::nullopt;
  }

  std::wstring value;
  if (storage->ReadValue(kHnsWireguardConfigKeyName, &value) !=
          ERROR_SUCCESS ||
      value.empty()) {
    return absl::nullopt;
  }
  return base::FilePath(value);
}

bool UpdateLastUsedConfigPath(const base::FilePath& config_path) {
  base::win::RegKey storage;
  if (storage.Create(HKEY_LOCAL_MACHINE,
                     GetHnsVpnWireguardServiceRegistryStoragePath().c_str(),
                     KEY_SET_VALUE) != ERROR_SUCCESS) {
    return false;
  }
  if (storage.WriteValue(kHnsWireguardConfigKeyName,
                         config_path.value().c_str()) != ERROR_SUCCESS) {
    return false;
  }
  return true;
}

void RemoveStorageKey() {
  if (base::win::RegKey(HKEY_LOCAL_MACHINE,
                        kHnsVpnWireguardServiceRegistryStoragePath,
                        KEY_ALL_ACCESS)
          .DeleteKey(hns_vpn::GetHnsVpnWireguardServiceName().c_str()) !=
      ERROR_SUCCESS) {
    VLOG(1) << "Failed to delete storage registry value";
  }
}

}  // namespace wireguard

bool IsVPNTrayIconEnabled() {
  auto storage = GetStorageKey(HKEY_CURRENT_USER, KEY_QUERY_VALUE);
  if (!storage.has_value()) {
    return true;
  }

  DWORD value = 1;
  if (storage->ReadValueDW(kHnsWireguardEnableTrayIconKeyName, &value) !=
      ERROR_SUCCESS) {
    return true;
  }
  return value == 1;
}

void EnableVPNTrayIcon(bool value) {
  auto storage = GetStorageKey(HKEY_CURRENT_USER, KEY_SET_VALUE);
  if (!storage.has_value()) {
    return;
  }

  if (storage->WriteValue(kHnsWireguardEnableTrayIconKeyName, DWORD(value)) !=
      ERROR_SUCCESS) {
    VLOG(1) << "False to write registry value";
  }
}

void SetWireguardActive(bool value) {
  auto storage = GetStorageKey(HKEY_CURRENT_USER, KEY_SET_VALUE);
  if (!storage.has_value()) {
    return;
  }

  if (storage->WriteValue(kHnsWireguardActiveKeyName, DWORD(value)) !=
      ERROR_SUCCESS) {
    VLOG(1) << "False to write registry value";
  }
}

bool IsWireguardActive() {
  auto storage = GetStorageKey(HKEY_CURRENT_USER, KEY_QUERY_VALUE);
  if (!storage.has_value()) {
    return true;
  }

  DWORD value = 1;
  if (storage->ReadValueDW(kHnsWireguardActiveKeyName, &value) !=
      ERROR_SUCCESS) {
    return true;
  }
  return value == 1;
}

// If the tunnel service failed to launch or crashed more than the limit we
// should ask user for the fallback to IKEv2 implementation.
bool ShouldFallbackToIKEv2() {
  auto storage = GetStorageKey(HKEY_LOCAL_MACHINE, KEY_READ);
  if (!storage.has_value()) {
    return true;
  }

  DWORD launch = 0;
  storage->ReadValueDW(kHnsVpnWireguardCounterOfTunnelUsage, &launch);
  return launch >= kHnsVpnWireguardMaxFailedAttempts ||
         !wireguard::IsWireguardServiceRegistered();
}

// Increments number of usages for the wireguard tunnel service.
void IncrementWireguardTunnelUsageFlag() {
  base::win::RegKey key(
      HKEY_LOCAL_MACHINE,
      wireguard::GetHnsVpnWireguardServiceRegistryStoragePath().c_str(),
      KEY_ALL_ACCESS);
  if (!key.Valid()) {
    VLOG(1) << "Failed to open wireguard service storage";
    return;
  }
  DWORD launch = 0;
  key.ReadValueDW(kHnsVpnWireguardCounterOfTunnelUsage, &launch);
  launch++;
  key.WriteValue(kHnsVpnWireguardCounterOfTunnelUsage, launch);
}

// Resets number of launches for the wireguard tunnel service.
void ResetWireguardTunnelUsageFlag() {
  base::win::RegKey key(
      HKEY_LOCAL_MACHINE,
      wireguard::GetHnsVpnWireguardServiceRegistryStoragePath().c_str(),
      KEY_ALL_ACCESS);
  if (!key.Valid()) {
    VLOG(1) << "Failed to open vpn service storage";
    return;
  }
  key.DeleteValue(kHnsVpnWireguardCounterOfTunnelUsage);
}

void WriteConnectionState(int value) {
  auto storage = GetStorageKey(HKEY_CURRENT_USER, KEY_SET_VALUE);
  if (!storage.has_value()) {
    return;
  }
  if (storage->WriteValue(kHnsWireguardConnectionStateName, DWORD(value)) !=
      ERROR_SUCCESS) {
    VLOG(1) << "False to write registry value";
  }
}

absl::optional<int32_t> GetConnectionState() {
  auto storage = GetStorageKey(HKEY_CURRENT_USER, KEY_QUERY_VALUE);
  if (!storage.has_value()) {
    return absl::nullopt;
  }
  DWORD value;
  if (storage->ReadValueDW(kHnsWireguardConnectionStateName, &value) ==
      ERROR_SUCCESS) {
    return value;
  }
  return absl::nullopt;
}

}  // namespace hns_vpn
