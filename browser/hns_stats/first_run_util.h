/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_STATS_FIRST_RUN_UTIL_H_
#define HNS_BROWSER_HNS_STATS_FIRST_RUN_UTIL_H_

#include "base/time/time.h"

class PrefService;

namespace hns_stats {

base::Time GetFirstRunTime(PrefService* local_state);

}  // namespace hns_stats

#endif  // HNS_BROWSER_HNS_STATS_FIRST_RUN_UTIL_H_
