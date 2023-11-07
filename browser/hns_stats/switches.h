// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_HNS_STATS_SWITCHES_H_
#define HNS_BROWSER_HNS_STATS_SWITCHES_H_

namespace hns_stats {

namespace switches {

// Allows setting the usage server to a custom host. Useful both for manual
// testing against staging and for browser tests.
constexpr char kHnsStatsUpdaterServer[] =
    "hns-stats-updater-server";

}  // namespace switches
}  // namespace hns_stats

#endif  // HNS_BROWSER_HNS_STATS_SWITCHES_H_
