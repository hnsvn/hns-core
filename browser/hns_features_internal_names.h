/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_FEATURES_INTERNAL_NAMES_H_
#define HNS_BROWSER_HNS_FEATURES_INTERNAL_NAMES_H_

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "build/build_config.h"

constexpr char kPlaylistFeatureInternalName[] = "playlist";
constexpr char kPlaylistFakeUAFeatureInternalName[] = "playlist-fake-ua";
#if BUILDFLAG(ENABLE_HNS_VPN)
constexpr char kHnsVPNFeatureInternalName[] = "hns-vpn";
#if BUILDFLAG(IS_WIN)
constexpr char kHnsVPNDnsFeatureInternalName[] = "hns-vpn-dns";
constexpr char kHnsVPNWireguardFeatureInternalName[] = "hns-vpn-wireguard";
#endif
#endif

#endif  // HNS_BROWSER_HNS_FEATURES_INTERNAL_NAMES_H_
