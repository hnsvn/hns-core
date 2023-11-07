/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_FEATURES_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_FEATURES_H_

#include "base/feature_list.h"
#include "build/build_config.h"

namespace hns_vpn {
namespace features {

BASE_DECLARE_FEATURE(kHnsVPN);
BASE_DECLARE_FEATURE(kHnsVPNLinkSubscriptionAndroidUI);
#if BUILDFLAG(IS_WIN)
BASE_DECLARE_FEATURE(kHnsVPNDnsProtection);
BASE_DECLARE_FEATURE(kHnsVPNUseWireguardService);
#endif
}  // namespace features
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_FEATURES_H_
