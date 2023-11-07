// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_HNS_NEWS_P3A_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_HNS_NEWS_P3A_H_

#include <cstdint>

class PrefRegistrySimple;
class PrefService;

namespace hns_news {
namespace p3a {

extern const char kWeeklySessionCountHistogramName[];
extern const char kTotalCardViewsHistogramName[];
extern const char kWeeklyDisplayAdsViewedHistogramName[];
extern const char kDirectFeedsTotalHistogramName[];
extern const char kWeeklyAddedDirectFeedsHistogramName[];
extern const char kLastUsageTimeHistogramName[];
extern const char kNewUserReturningHistogramName[];
extern const char kIsEnabledHistogramName[];
extern const char kUsageMonthlyHistogramName[];
extern const char kUsageDailyHistogramName[];

void RecordAtInit(PrefService* prefs);
void RecordAtSessionStart(PrefService* prefs);

void RecordWeeklyDisplayAdsViewedCount(PrefService* prefs, bool is_add);
void RecordWeeklyAddedDirectFeedsCount(PrefService* prefs, int change);
void RecordDirectFeedsTotal(PrefService* prefs);

void RecordTotalCardViews(PrefService* prefs, uint64_t count_delta);
void RecordFeatureEnabledChange(PrefService* prefs);

void RegisterProfilePrefs(PrefRegistrySimple* registry);
void RegisterProfilePrefsForMigration(PrefRegistrySimple* registry);
void MigrateObsoleteProfilePrefs(PrefService* prefs);

}  // namespace p3a
}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_HNS_NEWS_P3A_H_
