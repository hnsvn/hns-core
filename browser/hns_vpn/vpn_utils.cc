/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/vpn_utils.h"

#include "hns/browser/profiles/profile_util.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "build/build_config.h"
#include "components/user_prefs/user_prefs.h"

namespace hns_vpn {

bool IsAllowedForContext(content::BrowserContext* context) {
  return hns::IsRegularProfile(context) &&
         hns_vpn::IsHnsVPNFeatureEnabled();
}

bool IsHnsVPNEnabled(content::BrowserContext* context) {
  // TODO(simonhong): Can we use this check for android?
  // For now, vpn is disabled by default on desktop but not sure on
  // android.
#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC)
  return hns_vpn::IsHnsVPNEnabled(user_prefs::UserPrefs::Get(context)) &&
         IsAllowedForContext(context);
#else
  return hns::IsRegularProfile(context);
#endif
}

}  // namespace hns_vpn
