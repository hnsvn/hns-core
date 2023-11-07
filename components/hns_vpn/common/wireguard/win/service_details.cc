/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/wireguard/win/service_details.h"

#include <guiddef.h>

#include "base/containers/cxx20_erase.h"
#include "base/path_service.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_constants.h"
#include "build/build_config.h"
#include "chrome/install_static/install_util.h"
#include "components/version_info/version_info.h"

namespace hns_vpn {

namespace {

// The service installed to %(VersionDir)s\HnsVpnWireguardService
constexpr wchar_t kHnsVpnWireguardServiceSubFolder[] =
    L"HnsVpnWireguardService";

// 053057AB-CF06-4E6C-BBAD-F8DA6436D933
constexpr IID kHnsWireguardServiceIID = {
    0x053057ab,
    0xcf06,
    0x4e6c,
    {0xbb, 0xad, 0xf8, 0xda, 0x64, 0x36, 0xd9, 0x33}};

#if BUILDFLAG(CHANNEL_NIGHTLY)
constexpr wchar_t kHnsWireguardTunnelServiceName[] =
    L"HnsVpnNightlyWireguardTunnelService";
// A8D57D90-7A29-4405-91D7-A712F347E426
constexpr CLSID kHnsWireguardServiceCLSID = {
    0xa8d57d90,
    0x7a29,
    0x4405,
    {0x91, 0xd7, 0xa7, 0x12, 0xf3, 0x47, 0xe4, 0x26}};
#elif BUILDFLAG(CHANNEL_BETA)
constexpr wchar_t kHnsWireguardTunnelServiceName[] =
    L"HnsVpnBetaWireguardTunnelService";
// 93175676-5FAC-4D73-B1E1-5485003C9427
constexpr CLSID kHnsWireguardServiceCLSID = {
    0x93175676,
    0x5fac,
    0x4d73,
    {0xb1, 0xe1, 0x54, 0x85, 0x00, 0x3c, 0x94, 0x27}};
#elif BUILDFLAG(CHANNEL_DEV)
constexpr wchar_t kHnsWireguardTunnelServiceName[] =
    L"HnsVpnDevWireguardTunnelService";
// 52C95DE1-D7D9-4C03-A275-8A4517AFAE08
constexpr CLSID kHnsWireguardServiceCLSID = {
    0x52c95de1,
    0xd7d9,
    0x4c03,
    {0xa2, 0x75, 0x8a, 0x45, 0x17, 0xaf, 0xae, 0x08}};
#elif BUILDFLAG(CHANNEL_DEVELOPMENT)
constexpr wchar_t kHnsWireguardTunnelServiceName[] =
    L"HnsVpnDevelopmentWireguardTunnelService";
// 57B73EDD-CBE4-46CA-8ACB-11D90840AF6E
constexpr CLSID kHnsWireguardServiceCLSID = {
    0x57b73edd,
    0xcbe4,
    0x46ca,
    {0x8a, 0xcb, 0x11, 0xd9, 0x08, 0x40, 0xaf, 0x6e}};
#else
constexpr wchar_t kHnsWireguardTunnelServiceName[] =
    L"HnsVpnWireguardTunnelService";

// 088C5F6E-B213-4A8E-98AD-9D64D8913968
constexpr CLSID kHnsWireguardServiceCLSID = {
    0x088c5f6e,
    0xb213,
    0x4a8e,
    {0x98, 0xad, 0x9d, 0x64, 0xd8, 0x91, 0x39, 0x68}};
#endif

}  // namespace

// Returns the Hns Vpn Service CLSID, IID, Name, and Display Name
// respectively.
const CLSID& GetHnsVpnWireguardServiceClsid() {
  return kHnsWireguardServiceCLSID;
}

const IID& GetHnsVpnWireguardServiceIid() {
  return kHnsWireguardServiceIID;
}

std::wstring GetHnsVpnWireguardServiceDisplayName() {
  static constexpr wchar_t kHnsWireguardServiceDisplayName[] =
      L" Vpn Wireguard Service";
  return install_static::GetBaseAppName() + kHnsWireguardServiceDisplayName;
}

std::wstring GetHnsVpnWireguardServiceName() {
  std::wstring name = GetHnsVpnWireguardServiceDisplayName();
  base::EraseIf(name, isspace);
  return name;
}

std::wstring GetHnsVpnWireguardTunnelServiceName() {
  return kHnsWireguardTunnelServiceName;
}

base::FilePath GetHnsVPNWireguardServiceInstallationPath(
    const base::FilePath& target_path,
    const base::Version& version) {
  return target_path.AppendASCII(version.GetString())
      .Append(hns_vpn::kHnsVpnWireguardServiceSubFolder)
      .Append(hns_vpn::kHnsVpnWireguardServiceExecutable);
}

base::FilePath GetHnsVPNWireguardServiceExecutablePath() {
  base::FilePath exe_dir;
  base::PathService::Get(base::DIR_EXE, &exe_dir);
  return version_info::IsOfficialBuild()
             ? hns_vpn::GetHnsVPNWireguardServiceInstallationPath(
                   exe_dir, version_info::GetVersion())
             : exe_dir.Append(hns_vpn::kHnsVpnWireguardServiceExecutable);
}

}  // namespace hns_vpn
