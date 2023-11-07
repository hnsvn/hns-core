/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_service/service/install_utils.h"

#include "base/base_paths.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/win/registry.h"
#include "base/win/windows_types.h"
#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_service/service/wireguard_tunnel_service.h"
#include "hns/components/hns_vpn/common/win/scoped_sc_handle.h"
#include "hns/components/hns_vpn/common/win/utils.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_constants.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_details.h"
#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"
#include "chrome/installer/util/install_service_work_item.h"

namespace hns_vpn {

namespace {

constexpr wchar_t kAutoRunKeyPath[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

bool RemoveWireguardConfigDirectory(const base::FilePath& last_used_config) {
  auto wireguard_config_folder = last_used_config.DirName();
  if (wireguard_config_folder.empty() ||
      !base::PathExists(wireguard_config_folder)) {
    return true;
  }

  return base::DeletePathRecursively(wireguard_config_folder);
}

void AddToStartup(const std::wstring& value,
                  const base::CommandLine& command_line) {
  base::win::RegKey key(HKEY_LOCAL_MACHINE, kAutoRunKeyPath, KEY_WRITE);
  if (!key.Valid()) {
    VLOG(1) << "Failed to write wireguard service to startup";
    return;
  }
  key.WriteValue(value.c_str(), command_line.GetCommandLineString().c_str());
}

void RemoveFromStartup(const std::wstring& value) {
  base::win::RegKey key(HKEY_LOCAL_MACHINE, kAutoRunKeyPath, KEY_WRITE);
  if (!key.Valid()) {
    VLOG(1) << "Failed to write wireguard service to startup";
    return;
  }
  key.DeleteValue(value.c_str());
}
}  // namespace

bool ConfigureHnsWireguardService(const std::wstring& service_name) {
  ScopedScHandle scm(::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS));
  if (!scm.IsValid()) {
    VLOG(1) << "::OpenSCManager failed. service_name: " << service_name
            << ", error: " << std::hex << HRESULTFromLastError();
    return false;
  }
  base::FilePath exe_path;
  if (!base::PathService::Get(base::FILE_EXE, &exe_path)) {
    return S_OK;
  }

  ScopedScHandle service(
      ::OpenService(scm.Get(), service_name.c_str(), SERVICE_ALL_ACCESS));
  if (!service.IsValid()) {
    VLOG(1) << "Failed to create service_name: " << service_name
            << ", error: " << std::hex << HRESULTFromLastError();
    return false;
  }
  SERVICE_SID_INFO info = {};
  info.dwServiceSidType = SERVICE_SID_TYPE_UNRESTRICTED;
  if (!ChangeServiceConfig2(service.Get(), SERVICE_CONFIG_SERVICE_SID_INFO,
                            &info)) {
    VLOG(1) << "ChangeServiceConfig2 failed:" << std::hex
            << HRESULTFromLastError();
    return false;
  }
  return true;
}

// Installs Hns VPN Wireguard Windows service and configures the service
// config.
bool InstallHnsWireguardService() {
  base::FilePath exe_dir;
  base::PathService::Get(base::DIR_EXE, &exe_dir);
  base::CommandLine service_cmd(
      exe_dir.Append(hns_vpn::kHnsVpnWireguardServiceExecutable));
  installer::InstallServiceWorkItem install_service_work_item(
      hns_vpn::GetHnsVpnWireguardServiceName(),
      hns_vpn::GetHnsVpnWireguardServiceDisplayName(), SERVICE_DEMAND_START,
      service_cmd, base::CommandLine(base::CommandLine::NO_PROGRAM),
      hns_vpn::wireguard::GetHnsVpnWireguardServiceRegistryStoragePath(),
      {hns_vpn::GetHnsVpnWireguardServiceClsid()},
      {hns_vpn::GetHnsVpnWireguardServiceIid()});
  install_service_work_item.set_best_effort(true);
  install_service_work_item.set_rollback_enabled(false);
  if (install_service_work_item.Do()) {
    auto success = hns_vpn::ConfigureHnsWireguardService(
        hns_vpn::GetHnsVpnWireguardServiceName());
    if (success) {
      service_cmd.AppendSwitch(
          hns_vpn::kHnsVpnWireguardServiceInteractiveSwitchName);
      AddToStartup(hns_vpn::GetHnsVpnWireguardServiceName().c_str(),
                   service_cmd);
    }
    return success;
  }
  return false;
}

// Uninstalling and clearing Hns VPN service data.
bool UninstallHnsWireguardService() {
  hns_vpn::wireguard::RemoveExistingWireguardService();
  auto last_used_config = hns_vpn::wireguard::GetLastUsedConfigPath();
  if (last_used_config.has_value() &&
      !RemoveWireguardConfigDirectory(last_used_config.value())) {
    LOG(WARNING) << "Failed to delete config directory"
                 << last_used_config.value().DirName();
  }
  RemoveFromStartup(hns_vpn::GetHnsVpnWireguardServiceName().c_str());
  wireguard::RemoveStorageKey();

  if (!installer::InstallServiceWorkItem::DeleteService(
          hns_vpn::GetHnsVpnWireguardServiceName(),
          hns_vpn::wireguard::
              GetHnsVpnWireguardServiceRegistryStoragePath(),
          {}, {})) {
    LOG(WARNING) << "Failed to delete "
                 << hns_vpn::GetHnsVpnWireguardServiceName();
    return false;
  }
  return true;
}

}  // namespace hns_vpn
