/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/p3a/p3a_core_metrics.h"

#include <utility>

#include "base/metrics/histogram_macros.h"
#include "base/time/time.h"
#include "hns/components/p3a_utils/bucket.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace hns {

namespace {

HnsWindowTracker* g_hns_windows_tracker_instance = nullptr;

constexpr char kLastTimeIncognitoUsed[] =
    "core_p3a_metrics.incognito_used_timestamp";
constexpr char kTorUsed[] = "core_p3a_metrics.tor_used";

constexpr size_t kWindowUsageP3AIntervalMinutes = 10;

// Note: append-only enumeration! Never remove any existing values, as this enum
// is used to bucket a UMA histogram, and removing values breaks that.
// Each subsequent "bucket" doesn't include previous bucket (i.e. if the window
// was used 5 days ago, the proper bucket is |kUsedInLastWeek|, not
// |kUsedInLast28Days|.
enum class WindowUsageStats {
  kUsedIn24h,
  kUsedInLastWeek,
  kUsedInLast28Days,
  kEverUsed,
  kNeverUsed,
  kSize,
};

const char* GetPrefNameForProfile(Profile* profile) {
  if (profile->IsIncognitoProfile() &&
      !profile->IsTor()) {
    return kLastTimeIncognitoUsed;
  }
  return nullptr;
}

HnsUptimeTracker* g_hns_uptime_tracker_instance = nullptr;

constexpr base::TimeDelta kUsageTimeQueryInterval = base::Minutes(1);
constexpr base::TimeDelta kUsageTimeReportInterval = base::Days(1);
constexpr char kDailyUptimesListPrefName[] = "daily_uptimes";  // DEPRECATED
constexpr char kDailyUptimeSumPrefName[] = "hns.misc_metrics.uptime_sum";
constexpr char kDailyUptimeFrameStartTimePrefName[] =
    "hns.misc_metrics.uptime_frame_start_time";

constexpr char kBrowserOpenTimeHistogramName[] = "Hns.Uptime.BrowserOpenTime";

constexpr int kBrowserOpenTimeBuckets[] = {30, 60, 120, 180, 300, 420, 600};

}  // namespace

HnsUptimeTracker::HnsUptimeTracker(PrefService* local_state)
    : local_state_(local_state),
      report_frame_start_time_(
          local_state->GetTime(kDailyUptimeFrameStartTimePrefName)),
      report_frame_time_sum_(
          local_state_->GetTimeDelta(kDailyUptimeSumPrefName)) {
  if (report_frame_start_time_.is_null()) {
    // If today is the first time monitoring uptime, set the frame start time
    // to now.
    ResetReportFrame();
  }
  RecordP3A();
  timer_.Start(FROM_HERE, kUsageTimeQueryInterval,
               base::BindRepeating(&HnsUptimeTracker::RecordUsage,
                                   base::Unretained(this)));
}

void HnsUptimeTracker::RecordUsage() {
  const base::TimeDelta new_total = usage_clock_.GetTotalUsageTime();
  const base::TimeDelta total_diff = new_total - current_total_usage_;
  if (total_diff > base::TimeDelta()) {
    report_frame_time_sum_ += total_diff;
    current_total_usage_ = new_total;
    local_state_->SetTimeDelta(kDailyUptimeSumPrefName, report_frame_time_sum_);

    RecordP3A();
  }
}

void HnsUptimeTracker::RecordP3A() {
  if ((base::Time::Now() - report_frame_start_time_) <
      kUsageTimeReportInterval) {
    // Do not report, since 1 day has not passed.
    return;
  }
  p3a_utils::RecordToHistogramBucket(kBrowserOpenTimeHistogramName,
                                     kBrowserOpenTimeBuckets,
                                     report_frame_time_sum_.InMinutes());
  ResetReportFrame();
}

void HnsUptimeTracker::ResetReportFrame() {
  report_frame_time_sum_ = base::TimeDelta();
  report_frame_start_time_ = base::Time::Now();
  local_state_->SetTimeDelta(kDailyUptimeSumPrefName, report_frame_time_sum_);
  local_state_->SetTime(kDailyUptimeFrameStartTimePrefName,
                        report_frame_start_time_);
}

HnsUptimeTracker::~HnsUptimeTracker() = default;

void HnsUptimeTracker::CreateInstance(PrefService* local_state) {
  g_hns_uptime_tracker_instance = new HnsUptimeTracker(local_state);
}

void HnsUptimeTracker::RegisterPrefs(PrefRegistrySimple* registry) {
  registry->RegisterTimeDeltaPref(kDailyUptimeSumPrefName, base::TimeDelta());
  registry->RegisterTimePref(kDailyUptimeFrameStartTimePrefName, base::Time());
}

void HnsUptimeTracker::RegisterPrefsForMigration(
    PrefRegistrySimple* registry) {
  // Added 10/2023
  registry->RegisterListPref(kDailyUptimesListPrefName);
}

void HnsUptimeTracker::MigrateObsoletePrefs(PrefService* local_state) {
  // Added 10/2023
  local_state->ClearPref(kDailyUptimesListPrefName);
}

HnsWindowTracker::HnsWindowTracker(PrefService* local_state)
    : local_state_(local_state) {
  if (!local_state) {
    // Can happen in tests.
    return;
  }
  BrowserList::AddObserver(this);
  timer_.Start(FROM_HERE, base::Minutes(kWindowUsageP3AIntervalMinutes),
               base::BindRepeating(&HnsWindowTracker::UpdateP3AValues,
                                   base::Unretained(this)));
  UpdateP3AValues();
}

HnsWindowTracker::~HnsWindowTracker() {
  BrowserList::RemoveObserver(this);
}

void HnsWindowTracker::CreateInstance(PrefService* local_state) {
  g_hns_windows_tracker_instance = new HnsWindowTracker(local_state);
}

void HnsWindowTracker::RegisterPrefs(PrefRegistrySimple* registry) {
  registry->RegisterTimePref(kLastTimeIncognitoUsed, {});
  registry->RegisterBooleanPref(kTorUsed, false);
}

void HnsWindowTracker::OnBrowserAdded(Browser* browser) {
  if (browser->profile()->IsTor()) {
    local_state_->SetBoolean(kTorUsed, true);
    return;
  }
  const char* pref = GetPrefNameForProfile(browser->profile());
  if (pref) {
    local_state_->SetTime(pref, base::Time::Now());
  }
}

void HnsWindowTracker::OnBrowserSetLastActive(Browser* browser) {
  const char* pref = GetPrefNameForProfile(browser->profile());
  if (pref) {
    local_state_->SetTime(pref, base::Time::Now());
  }
}

void HnsWindowTracker::UpdateP3AValues() const {
  // Deal with the incognito window.
  WindowUsageStats bucket;
  const base::Time time = local_state_->GetTime(kLastTimeIncognitoUsed);
  const base::Time now = base::Time::Now();
  if (time.is_null()) {
    bucket = WindowUsageStats::kNeverUsed;
  } else if (now - time < base::Hours(24)) {
    bucket = WindowUsageStats::kUsedIn24h;
  } else if (now - time < base::Days(7)) {
    bucket = WindowUsageStats::kUsedInLastWeek;
  } else if (now - time < base::Days(28)) {
    bucket = WindowUsageStats::kUsedInLast28Days;
  } else {
    bucket = WindowUsageStats::kEverUsed;
  }

  UMA_HISTOGRAM_ENUMERATION("Hns.Core.LastTimeIncognitoUsed", bucket,
                            WindowUsageStats::kSize);

  // Record if the TOR window was ever used.
  // 0 -> Yes; 1 -> No.
  const int tor_used = !local_state_->GetBoolean(kTorUsed);
  UMA_HISTOGRAM_EXACT_LINEAR("Hns.Core.TorEverUsed", tor_used, 1);
}

}  // namespace hns
