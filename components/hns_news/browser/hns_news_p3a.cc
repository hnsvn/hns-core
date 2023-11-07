// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_news/browser/hns_news_p3a.h"

#include <algorithm>

#include "base/logging.h"
#include "base/metrics/histogram_macros.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/p3a_utils/bucket.h"
#include "hns/components/p3a_utils/feature_usage.h"
#include "hns/components/time_period_storage/weekly_storage.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace hns_news {
namespace p3a {

constexpr char kWeeklySessionCountHistogramName[] =
    "Hns.Today.WeeklySessionCount";
constexpr char kTotalCardViewsHistogramName[] =
    "Hns.Today.WeeklyTotalCardViews";
constexpr char kWeeklyDisplayAdsViewedHistogramName[] =
    "Hns.Today.WeeklyDisplayAdsViewedCount";
constexpr char kDirectFeedsTotalHistogramName[] =
    "Hns.Today.DirectFeedsTotal";
constexpr char kWeeklyAddedDirectFeedsHistogramName[] =
    "Hns.Today.WeeklyAddedDirectFeedsCount";
constexpr char kLastUsageTimeHistogramName[] = "Hns.Today.LastUsageTime";
constexpr char kNewUserReturningHistogramName[] =
    "Hns.Today.NewUserReturning";
constexpr char kIsEnabledHistogramName[] = "Hns.Today.IsEnabled";
constexpr char kUsageMonthlyHistogramName[] = "Hns.Today.UsageMonthly";
constexpr char kUsageDailyHistogramName[] = "Hns.Today.UsageDaily";

namespace {

bool IsNewsEnabled(PrefService* prefs) {
  return prefs->GetBoolean(prefs::kHnsNewsOptedIn) &&
         prefs->GetBoolean(prefs::kNewTabPageShowToday);
}

uint64_t AddToWeeklyStorageAndGetSum(PrefService* prefs,
                                     const char* pref_name,
                                     int change) {
  WeeklyStorage storage(prefs, pref_name);
  if (change > 0) {
    storage.AddDelta(1);
  } else if (change < 0) {
    storage.SubDelta(1);
  }
  return storage.GetWeeklySum();
}

void RecordLastUsageTime(PrefService* prefs) {
  p3a_utils::RecordFeatureLastUsageTimeMetric(
      prefs, prefs::kHnsNewsLastSessionTime, kLastUsageTimeHistogramName);
}

void RecordNewUserReturning(PrefService* prefs) {
  p3a_utils::RecordFeatureNewUserReturning(
      prefs, prefs::kHnsNewsFirstSessionTime,
      prefs::kHnsNewsLastSessionTime, prefs::kHnsNewsUsedSecondDay,
      kNewUserReturningHistogramName);
}

void RecordWeeklySessionCount(PrefService* prefs, bool is_add) {
  // Track how many times in the past week
  // user has scrolled to Hns News.
  constexpr int buckets[] = {0, 1, 3, 7, 12, 18, 25, 1000};
  uint64_t total_session_count = AddToWeeklyStorageAndGetSum(
      prefs, prefs::kHnsNewsWeeklySessionCount, is_add);
  p3a_utils::RecordToHistogramBucket(kWeeklySessionCountHistogramName, buckets,
                                     total_session_count);
}

void RecordGeneralUsage() {
  UMA_HISTOGRAM_BOOLEAN(kUsageMonthlyHistogramName, true);
  UMA_HISTOGRAM_BOOLEAN(kUsageDailyHistogramName, true);
}

}  // namespace

void RecordAtSessionStart(PrefService* prefs) {
  p3a_utils::RecordFeatureUsage(prefs, prefs::kHnsNewsFirstSessionTime,
                                prefs::kHnsNewsLastSessionTime);

  RecordLastUsageTime(prefs);
  RecordNewUserReturning(prefs);
  RecordGeneralUsage();

  RecordWeeklySessionCount(prefs, true);
}

void RecordWeeklyDisplayAdsViewedCount(PrefService* prefs, bool is_add) {
  // Store current weekly total in p3a, ready to send on the next upload
  constexpr int buckets[] = {0, 1, 4, 8, 14, 30, 60, 120};
  uint64_t total = AddToWeeklyStorageAndGetSum(
      prefs, prefs::kHnsNewsWeeklyDisplayAdViewedCount, is_add);
  p3a_utils::RecordToHistogramBucket(kWeeklyDisplayAdsViewedHistogramName,
                                     buckets, total);
}

void RecordDirectFeedsTotal(PrefService* prefs) {
  constexpr int buckets[] = {0, 1, 2, 3, 4, 5, 10};
  const auto& direct_feeds_dict = prefs->GetDict(prefs::kHnsNewsDirectFeeds);
  std::size_t feed_count = direct_feeds_dict.size();
  p3a_utils::RecordToHistogramBucket(kDirectFeedsTotalHistogramName, buckets,
                                     feed_count);
}

void RecordWeeklyAddedDirectFeedsCount(PrefService* prefs, int change) {
  constexpr int buckets[] = {0, 1, 2, 3, 4, 5, 10};
  uint64_t weekly_total = AddToWeeklyStorageAndGetSum(
      prefs, prefs::kHnsNewsWeeklyAddedDirectFeedsCount, change);

  p3a_utils::RecordToHistogramBucket(kWeeklyAddedDirectFeedsHistogramName,
                                     buckets, weekly_total);
}

void RecordTotalCardViews(PrefService* prefs, uint64_t count_delta) {
  WeeklyStorage total_storage(prefs, prefs::kHnsNewsTotalCardViews);

  total_storage.AddDelta(count_delta);

  uint64_t total = total_storage.GetWeeklySum();

  int buckets[] = {0, 1, 10, 20, 40, 80, 100};
  VLOG(1) << "NewsP3A: total card views update: total = " << total
          << " count delta = " << count_delta;
  p3a_utils::RecordToHistogramBucket(kTotalCardViewsHistogramName, buckets,
                                     total);
}

void RecordFeatureEnabledChange(PrefService* prefs) {
  bool enabled = IsNewsEnabled(prefs);
  bool was_ever_enabled = prefs->GetBoolean(prefs::kHnsNewsWasEverEnabled);
  if (!enabled && !was_ever_enabled) {
    // If the user clicked "no thanks" on the NTP, then we don't want to record
    // this as an opt-out, since they were never opted in.
    return;
  }
  prefs->SetBoolean(prefs::kHnsNewsWasEverEnabled, true);
  UMA_HISTOGRAM_BOOLEAN(kIsEnabledHistogramName, enabled);
}

void RecordAtInit(PrefService* prefs) {
  RecordLastUsageTime(prefs);
  RecordNewUserReturning(prefs);

  RecordDirectFeedsTotal(prefs);
  RecordWeeklyAddedDirectFeedsCount(prefs, 0);
  RecordWeeklySessionCount(prefs, false);
  RecordWeeklyDisplayAdsViewedCount(prefs, false);
  RecordTotalCardViews(prefs, 0);

  if (IsNewsEnabled(prefs)) {
    prefs->SetBoolean(prefs::kHnsNewsWasEverEnabled, true);
  }
}

void RegisterProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterListPref(prefs::kHnsNewsWeeklySessionCount);
  registry->RegisterListPref(prefs::kHnsNewsWeeklyDisplayAdViewedCount);
  registry->RegisterListPref(prefs::kHnsNewsWeeklyAddedDirectFeedsCount);
  registry->RegisterListPref(prefs::kHnsNewsTotalCardViews);
  p3a_utils::RegisterFeatureUsagePrefs(
      registry, prefs::kHnsNewsFirstSessionTime,
      prefs::kHnsNewsLastSessionTime, prefs::kHnsNewsUsedSecondDay, nullptr,
      nullptr);
  registry->RegisterBooleanPref(prefs::kHnsNewsWasEverEnabled, false);
}

void RegisterProfilePrefsForMigration(PrefRegistrySimple* registry) {
  // added 05/2023
  registry->RegisterListPref(prefs::kHnsNewsWeeklyCardViewsCount);
  registry->RegisterListPref(prefs::kHnsNewsWeeklyCardVisitsCount);
  registry->RegisterUint64Pref(prefs::kHnsNewsCurrSessionCardViews, 0);
  registry->RegisterListPref(prefs::kHnsNewsDaysInMonthUsedCount);
}

void MigrateObsoleteProfilePrefs(PrefService* prefs) {
  // added 05/2023
  prefs->ClearPref(prefs::kHnsNewsWeeklyCardViewsCount);
  prefs->ClearPref(prefs::kHnsNewsWeeklyCardVisitsCount);
  prefs->ClearPref(prefs::kHnsNewsCurrSessionCardViews);
  prefs->ClearPref(prefs::kHnsNewsDaysInMonthUsedCount);
}

}  // namespace p3a
}  // namespace hns_news
