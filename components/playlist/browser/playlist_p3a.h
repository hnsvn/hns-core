/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_PLAYLIST_BROWSER_PLAYLIST_P3A_H_
#define HNS_COMPONENTS_PLAYLIST_BROWSER_PLAYLIST_P3A_H_

#include "base/memory/raw_ptr.h"
#include "base/time/time.h"
#include "base/timer/wall_clock_timer.h"

class PrefService;

namespace playlist {

extern const char kLastUsageTimeHistogramName[];
extern const char kFirstTimeOffsetHistogramName[];
extern const char kUsageDaysInWeekHistogramName[];
extern const char kNewUserReturningHistogramName[];

// Manages P3A metrics for playlist
class PlaylistP3A {
 public:
  PlaylistP3A(PrefService* local_state, base::Time browser_first_run_time);
  ~PlaylistP3A();
  PlaylistP3A(const PlaylistP3A&) = delete;
  PlaylistP3A& operator=(PlaylistP3A&) = delete;

  void ReportNewUsage();

 private:
  void SetUpTimer();
  void Update(bool new_usage);

  raw_ptr<PrefService> local_state_;
  base::Time browser_first_run_time_;
  base::WallClockTimer update_timer_;
};

}  // namespace playlist

#endif  // HNS_COMPONENTS_PLAYLIST_BROWSER_PLAYLIST_P3A_H_
