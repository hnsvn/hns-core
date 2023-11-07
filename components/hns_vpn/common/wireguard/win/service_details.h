/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_DETAILS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_DETAILS_H_

#include <guiddef.h>
#include <string>

#include "base/files/file_path.h"
#include "base/version.h"

namespace hns_vpn {
const CLSID& GetHnsVpnWireguardServiceClsid();
const IID& GetHnsVpnWireguardServiceIid();
std::wstring GetHnsVpnWireguardTunnelServiceName();
std::wstring GetHnsVpnWireguardServiceName();
std::wstring GetHnsVpnWireguardServiceDisplayName();
base::FilePath GetHnsVPNWireguardServiceInstallationPath(
    const base::FilePath& target_path,
    const base::Version& version);
base::FilePath GetHnsVPNWireguardServiceExecutablePath();
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_WIREGUARD_WIN_SERVICE_DETAILS_H_
