/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_STATS_BROWSER_HNS_STATS_UPDATER_OBSERVER_H_
#define HNS_COMPONENTS_HNS_STATS_BROWSER_HNS_STATS_UPDATER_OBSERVER_H_

#include "base/observer_list_types.h"

namespace hns_stats {

class HnsStatsUpdaterObserver : public base::CheckedObserver {
 public:
  ~HnsStatsUpdaterObserver() override {}

  virtual void OnStatsPingFired() {}
};

}  // namespace hns_stats
#endif  // HNS_COMPONENTS_HNS_STATS_BROWSER_HNS_STATS_UPDATER_OBSERVER_H_
