/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_HNS_STATS_HNS_STATS_PREFS_H_
#define HNS_IOS_BROWSER_HNS_STATS_HNS_STATS_PREFS_H_

class PrefRegistrySimple;

namespace hns_stats {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry);

}  // namespace hns_stats

#endif  // HNS_IOS_BROWSER_HNS_STATS_HNS_STATS_PREFS_H_
