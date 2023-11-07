/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/ikev2/win/hns_vpn_helper/hns_vpn_helper_state.h"

#include <windows.h>

#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/strings/utf_string_conversions.h"
#include "base/win/registry.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/win/hns_vpn_helper/hns_vpn_helper_constants.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/win/scoped_sc_handle.h"
#include "hns/components/hns_vpn/common/win/utils.h"
#include "chrome/install_static/install_modes.h"
#include "chrome/install_static/install_util.h"

namespace hns_vpn {

bool IsHnsVPNHelperServiceInstalled() {
  ScopedScHandle scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT));
  if (!scm.IsValid()) {
    VLOG(1) << "::OpenSCManager failed. service_name: "
            << hns_vpn::GetHnsVpnHelperServiceName()
            << ", error: " << std::hex << HRESULTFromLastError();
    return false;
  }
  ScopedScHandle service(::OpenService(
      scm.Get(), hns_vpn::GetHnsVpnHelperServiceName().c_str(),
      SERVICE_QUERY_STATUS));

  // Service registered and has not exceeded the number of auto-configured
  // restarts.
  return service.IsValid();
}

std::wstring GetHnsVPNConnectionName() {
  return base::UTF8ToWide(
      hns_vpn::GetHnsVPNEntryName(install_static::GetChromeChannel()));
}

std::wstring GetHnsVpnHelperServiceName() {
  std::wstring name = GetHnsVpnHelperServiceDisplayName();
  name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
  return name;
}

std::wstring GetHnsVpnHelperServiceDisplayName() {
  static constexpr wchar_t kHnsVpnServiceDisplayName[] = L" Vpn Service";
  return install_static::GetBaseAppName() + kHnsVpnServiceDisplayName;
}

bool IsNetworkFiltersInstalled() {
  DCHECK(IsHnsVPNHelperServiceInstalled());
  base::win::RegKey service_storage_key(
      HKEY_LOCAL_MACHINE, hns_vpn::kHnsVpnHelperRegistryStoragePath,
      KEY_READ);
  if (!service_storage_key.Valid()) {
    return false;
  }
  DWORD current = -1;
  if (service_storage_key.ReadValueDW(
          hns_vpn::kHnsVpnHelperFiltersInstalledValue, &current) !=
      ERROR_SUCCESS) {
    return false;
  }
  return current > 0;
}

// The service starts under sytem user so we save crashes to
// %PROGRAMDATA%\HnsSoftware\{service name}\Crashpad
base::FilePath GetVpnHelperServiceProfileDir() {
  auto program_data = install_static::GetEnvironmentString("PROGRAMDATA");
  if (program_data.empty()) {
    return base::FilePath();
  }
  return base::FilePath(base::UTF8ToWide(program_data))
      .Append(install_static::kCompanyPathName)
      .Append(hns_vpn::GetHnsVpnHelperServiceName());
}

}  // namespace hns_vpn
