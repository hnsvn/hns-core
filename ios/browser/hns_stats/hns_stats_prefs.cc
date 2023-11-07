/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/hns_stats/hns_stats_prefs.h"

#include "base/time/time.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "components/prefs/pref_registry_simple.h"

namespace hns_stats {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterTimePref(kHnsWalletPingReportedUnlockTime, base::Time());
}

}  // namespace hns_stats
