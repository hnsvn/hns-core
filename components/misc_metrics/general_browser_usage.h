/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_MISC_METRICS_GENERAL_BROWSER_USAGE_H_
#define HNS_COMPONENTS_MISC_METRICS_GENERAL_BROWSER_USAGE_H_

#include <memory>

#include "base/timer/wall_clock_timer.h"

class PrefRegistrySimple;
class PrefService;
class ISOWeeklyStorage;

namespace misc_metrics {

extern const char kWeeklyUseHistogramName[];
extern const char kProfileCountHistogramName[];

class GeneralBrowserUsage {
 public:
  explicit GeneralBrowserUsage(PrefService* local_state);
  ~GeneralBrowserUsage();

  GeneralBrowserUsage(const GeneralBrowserUsage&) = delete;
  GeneralBrowserUsage& operator=(const GeneralBrowserUsage&) = delete;

  static void RegisterPrefs(PrefRegistrySimple* registry);

  void ReportProfileCount(size_t count);

 private:
  void ReportWeeklyUse();

  void SetUpUpdateTimer();

  void Update();

  std::unique_ptr<ISOWeeklyStorage> usage_storage_;

  base::WallClockTimer report_timer_;
};

}  // namespace misc_metrics

#endif  // HNS_COMPONENTS_MISC_METRICS_GENERAL_BROWSER_USAGE_H_
