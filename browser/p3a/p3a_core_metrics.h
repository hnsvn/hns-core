/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_P3A_P3A_CORE_METRICS_H_
#define HNS_BROWSER_P3A_P3A_CORE_METRICS_H_

// The classes below can be used on desktop only
// because BrowserListObserver is available on desktop only
// Hns.Uptime.BrowserOpenMinutes, Hns.Core.LastTimeIncognitoUsed and
// Hns.Core.TorEverUsed don't work on Android

#include "build/build_config.h"

#if BUILDFLAG(IS_ANDROID)
#error This file should only be included on desktop.
#endif

#include "base/memory/raw_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/resource_coordinator/usage_clock.h"
#include "chrome/browser/ui/browser_list_observer.h"

class PrefService;
class PrefRegistrySimple;

namespace hns {

class HnsUptimeTracker {
 public:
  explicit HnsUptimeTracker(PrefService* local_state);
  HnsUptimeTracker(const HnsUptimeTracker&) = delete;
  HnsUptimeTracker& operator=(const HnsUptimeTracker&) = delete;
  ~HnsUptimeTracker();

  static void CreateInstance(PrefService* local_state);

  static void RegisterPrefs(PrefRegistrySimple* registry);
  static void RegisterPrefsForMigration(PrefRegistrySimple* registry);
  static void MigrateObsoletePrefs(PrefService* local_state);

 private:
  void RecordUsage();
  void RecordP3A();

  void ResetReportFrame();

  raw_ptr<PrefService> local_state_;
  resource_coordinator::UsageClock usage_clock_;
  base::RepeatingTimer timer_;
  base::TimeDelta current_total_usage_;

  base::Time report_frame_start_time_;
  base::TimeDelta report_frame_time_sum_;
};

// HnsWindowTracker is under !OS_ANDROID guard because
// BrowserListObserver should only be only on desktop
// Hns.Uptime.BrowserOpenMinutes and Hns.Core.LastTimeIncognitoUsed
// don't work on Android
#if !BUILDFLAG(IS_ANDROID)
// Periodically records P3A stats (extracted from Local State) regarding the
// time when incognito windows were used.
// Used as a leaking singletone.
class HnsWindowTracker : public BrowserListObserver {
 public:
  explicit HnsWindowTracker(PrefService* local_state);
  HnsWindowTracker(const HnsWindowTracker&) = delete;
  HnsWindowTracker& operator=(const HnsWindowTracker&) = delete;
  ~HnsWindowTracker() override;

  static void CreateInstance(PrefService* local_state);

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  // BrowserListObserver:
  void OnBrowserAdded(Browser* browser) override;
  void OnBrowserSetLastActive(Browser* browser) override;

  void UpdateP3AValues() const;

  base::RepeatingTimer timer_;
  raw_ptr<PrefService> local_state_ = nullptr;
};
#endif  // !BUILDFLAG(IS_ANDROID)

}  // namespace hns

#endif  // HNS_BROWSER_P3A_P3A_CORE_METRICS_H_
