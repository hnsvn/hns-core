/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/features.h"

#include "base/feature_list.h"
#include "build/build_config.h"

namespace hns_vpn {

namespace features {

BASE_FEATURE(kHnsVPN,
             "HnsVPN",
#if !BUILDFLAG(IS_LINUX)
             base::FEATURE_ENABLED_BY_DEFAULT
#else
             base::FEATURE_DISABLED_BY_DEFAULT
#endif
);

BASE_FEATURE(kHnsVPNLinkSubscriptionAndroidUI,
             "HnsVPNLinkSubscriptionAndroidUI",
             base::FEATURE_ENABLED_BY_DEFAULT);

#if BUILDFLAG(IS_WIN)
BASE_FEATURE(kHnsVPNDnsProtection,
             "HnsVPNDnsProtection",
             base::FEATURE_ENABLED_BY_DEFAULT);
BASE_FEATURE(kHnsVPNUseWireguardService,
             "HnsVPNUseWireguardService",
             base::FEATURE_ENABLED_BY_DEFAULT);
#endif
}  // namespace features

}  // namespace hns_vpn
