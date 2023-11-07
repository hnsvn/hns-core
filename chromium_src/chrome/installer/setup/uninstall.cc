/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/files/file_util.h"
#include "base/process/launch.h"
#include "base/process/process.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/installer/util/hns_shell_util.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/browser/connection/ikev2/win/hns_vpn_helper/hns_vpn_helper_constants.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/win/hns_vpn_helper/hns_vpn_helper_state.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/win/ras_utils.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_constants.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_details.h"
#endif
#define UninstallProduct UninstallProduct_ChromiumImpl

#include "src/chrome/installer/setup/uninstall.cc"

#undef UninstallProduct

namespace installer {

namespace {

bool UninstallHnsVPNWireguardService(const base::FilePath& exe_path) {
  if (!base::PathExists(exe_path)) {
    return false;
  }
  base::CommandLine cmd(exe_path);
  cmd.AppendSwitch(hns_vpn::kHnsVpnWireguardServiceUnnstallSwitchName);
  base::LaunchOptions options = base::LaunchOptions();
  options.wait = true;
  return base::LaunchProcess(cmd, options).IsValid();
}

void DeleteHnsFileKeys(HKEY root) {
  // Delete Software\Classes\HnsXXXFile.
  std::wstring reg_prog_id(ShellUtil::kRegClasses);
  reg_prog_id.push_back(base::FilePath::kSeparators[0]);
  reg_prog_id.append(GetProgIdForFileType());
  DeleteRegistryKey(root, reg_prog_id, WorkItem::kWow64Default);

  // Cleanup OpenWithList and OpenWithProgids:
  // http://msdn.microsoft.com/en-us/library/bb166549
  std::wstring file_assoc_key;
  std::wstring open_with_progids_key;
  for (int i = 0; ShellUtil::kPotentialFileAssociations[i] != nullptr; ++i) {
    file_assoc_key.assign(ShellUtil::kRegClasses);
    file_assoc_key.push_back(base::FilePath::kSeparators[0]);
    file_assoc_key.append(ShellUtil::kPotentialFileAssociations[i]);
    file_assoc_key.push_back(base::FilePath::kSeparators[0]);

    open_with_progids_key.assign(file_assoc_key);
    open_with_progids_key.append(ShellUtil::kRegOpenWithProgids);
    if (ShouldUseFileTypeProgId(ShellUtil::kPotentialFileAssociations[i])) {
      DeleteRegistryValue(root, open_with_progids_key, WorkItem::kWow64Default,
                          GetProgIdForFileType());
    }
  }
}

}  // namespace

InstallStatus UninstallProduct(const ModifyParams& modify_params,
                               bool remove_all,
                               bool force_uninstall,
                               const base::CommandLine& cmd_line) {
  DeleteHnsFileKeys(HKEY_CURRENT_USER);

  const auto installer_state = modify_params.installer_state;
  const base::FilePath chrome_exe(
      installer_state->target_path().Append(installer::kChromeExe));
  const std::wstring suffix(
      ShellUtil::GetCurrentInstallationSuffix(chrome_exe));
  if (installer_state->system_install() ||
      (remove_all &&
       ShellUtil::QuickIsChromeRegisteredInHKLM(chrome_exe, suffix))) {
    DeleteHnsFileKeys(HKEY_LOCAL_MACHINE);
  }
#if BUILDFLAG(ENABLE_HNS_VPN)
  if (installer_state->system_install()) {
    if (!InstallServiceWorkItem::DeleteService(
            hns_vpn::GetHnsVpnHelperServiceName(),
            hns_vpn::kHnsVpnHelperRegistryStoragePath, {}, {})) {
      LOG(WARNING) << "Failed to delete "
                   << hns_vpn::GetHnsVpnHelperServiceName();
    }

    if (!UninstallHnsVPNWireguardService(
            hns_vpn::GetHnsVPNWireguardServiceInstallationPath(
                installer_state->target_path(),
                *modify_params.current_version))) {
      LOG(WARNING) << "Failed to delete "
                   << hns_vpn::GetHnsVpnWireguardServiceName();
    }
  }
  hns_vpn::ras::RemoveEntry(hns_vpn::GetHnsVPNConnectionName());
#endif
  return UninstallProduct_ChromiumImpl(modify_params, remove_all,
                                       force_uninstall, cmd_line);
}

}  // namespace installer
