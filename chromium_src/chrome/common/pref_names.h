/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_COMMON_PREF_NAMES_H_
#define HNS_CHROMIUM_SRC_CHROME_COMMON_PREF_NAMES_H_

#include "build/build_config.h"

namespace prefs {
// Used by HnsVpnDnsObserverService to set cloudflare server url when
// HnsVPN is connected, otherwise this pref is empty. Final decision
// about whether or not to override actual DNS state will be made in
// stub_resolver_config_reader.
inline constexpr char kHnsVpnDnsConfig[] = "hns.hns_vpn.dns_config";
}  // namespace prefs

#include "src/chrome/common/pref_names.h"  // IWYU pragma: export

#endif  // HNS_CHROMIUM_SRC_CHROME_COMMON_PREF_NAMES_H_
