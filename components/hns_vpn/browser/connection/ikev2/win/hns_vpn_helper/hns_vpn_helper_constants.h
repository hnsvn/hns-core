/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_HELPER_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_HELPER_CONSTANTS_H_

#include <guiddef.h>

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"

namespace hns_vpn {

constexpr char kHnsVpnHelperInstall[] = "install";
constexpr char kHnsVpnHelperCrashMe[] = "crash-me";
constexpr wchar_t kHnsVPNHelperExecutable[] = L"hns_vpn_helper.exe";
constexpr wchar_t kHnsVpnHelperFiltersInstalledValue[] = L"filters";
// Repeating interval to check the connection is live.
constexpr int kCheckConnectionIntervalInSeconds = 3;

/* UUID of WFP sublayer used by all instances */
#if BUILDFLAG(CHANNEL_NIGHTLY)
constexpr wchar_t kHnsVPNServiceFilter[] =
    L"Hns VPN Nightly Service DNS Filter";
constexpr wchar_t kHnsVpnHelperRegistryStoragePath[] =
    L"Software\\HnsSoftware\\Hns\\Vpn\\HelperServiceNightly";

// 23e10e29-eb83-4d2c-9d77-f6e9b547f39c
constexpr GUID kVpnDnsSublayerGUID = {
    0x23e10e29,
    0xeb83,
    0x4d2c,
    {0x9d, 0x77, 0xf6, 0xe9, 0xb5, 0x47, 0xf3, 0x9c}};
#elif BUILDFLAG(CHANNEL_BETA)
constexpr wchar_t kHnsVPNServiceFilter[] =
    L"Hns VPN Beta Service DNS Filter";
constexpr wchar_t kHnsVpnHelperRegistryStoragePath[] =
    L"Software\\HnsSoftware\\Hns\\Vpn\\HelperServiceBeta";

// fc5fb7bc-e313-4f5e-8052-fe8b150f7de0
constexpr GUID kVpnDnsSublayerGUID = {
    0xfc5fb7bc,
    0xe313,
    0x4f5e,
    {0x80, 0x52, 0xfe, 0x8b, 0x15, 0x0f, 0x7d, 0xe0}};
#elif BUILDFLAG(CHANNEL_DEV)
constexpr wchar_t kHnsVPNServiceFilter[] =
    L"Hns VPN Dev Service DNS Filter";
constexpr wchar_t kHnsVpnHelperRegistryStoragePath[] =
    L"Software\\HnsSoftware\\Hns\\Vpn\\HelperServiceDev";
// c448b198-729d-4a89-879b-1cf0cd2460c0
constexpr GUID kVpnDnsSublayerGUID = {
    0xc448b198,
    0x729d,
    0x4a89,
    {0x87, 0x9b, 0x1c, 0xf0, 0xcd, 0x24, 0x60, 0xc0}};
#elif BUILDFLAG(CHANNEL_DEVELOPMENT)
constexpr wchar_t kHnsVPNServiceFilter[] =
    L"Hns VPN Development Service DNS Filter";
constexpr wchar_t kHnsVpnHelperRegistryStoragePath[] =
    L"Software\\HnsSoftware\\Hns\\Vpn\\HelperServiceDevelopment";
// 9c14e1f7-692f-495b-95e8-008113d3c0d6
constexpr GUID kVpnDnsSublayerGUID = {
    0x9c14e1f7,
    0x692f,
    0x495b,
    {0x95, 0xe8, 0x00, 0x81, 0x13, 0xd3, 0xc0, 0xd6}};
#else
constexpr wchar_t kHnsVPNServiceFilter[] = L"Hns VPN Service DNS Filter";
constexpr wchar_t kHnsVpnHelperRegistryStoragePath[] =
    L"Software\\HnsSoftware\\Hns\\Vpn\\HelperService";
// 754b7cbd-cad3-474e-8d2c-054413fd4509
constexpr GUID kVpnDnsSublayerGUID = {
    0x754b7cbd,
    0xcad3,
    0x474e,
    {0x8d, 0x2c, 0x05, 0x44, 0x13, 0xfd, 0x45, 0x09}};
#endif
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_HELPER_CONSTANTS_H_
