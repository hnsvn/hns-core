// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_news/browser/hns_news_p3a.h"

#include "base/test/metrics/histogram_tester.h"
#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/time_period_storage/weekly_storage.h"
#include "components/prefs/scoped_user_pref_update.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_news {
namespace p3a {

class HnsNewsP3ATest : public testing::Test {
 public:
  HnsNewsP3ATest()
      : task_environment_(base::test::TaskEnvironment::TimeSource::MOCK_TIME) {}

 protected:
  void SetUp() override {
    PrefRegistrySimple* registry = pref_service_.registry();
    HnsNewsController::RegisterProfilePrefs(registry);
    task_environment_.AdvanceClock(base::Days(2));
  }

  PrefService* GetPrefs() { return &pref_service_; }

  int GetWeeklySum(const char* pref_name) {
    WeeklyStorage storage(&pref_service_, pref_name);
    return storage.GetWeeklySum();
  }

  content::BrowserTaskEnvironment task_environment_;
  base::HistogramTester histogram_tester_;

 private:
  TestingPrefServiceSimple pref_service_;
};

TEST_F(HnsNewsP3ATest, TestWeeklySessionCountBasic) {
  PrefService* prefs = GetPrefs();

  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 0, 1);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 2);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 1, 1);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 3);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 2, 1);
  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 4);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 2, 2);

  RecordAtSessionStart(prefs);
  RecordAtSessionStart(prefs);
  RecordAtSessionStart(prefs);
  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 8);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 3, 4);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklySessionCount), 7);
}

TEST_F(HnsNewsP3ATest, TestWeeklySessionCountTimeFade) {
  PrefService* prefs = GetPrefs();
  RecordAtSessionStart(prefs);
  RecordAtSessionStart(prefs);

  task_environment_.AdvanceClock(base::Days(2));
  RecordAtSessionStart(prefs);

  task_environment_.AdvanceClock(base::Days(2));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 4);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 2, 3);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklySessionCount), 3);

  task_environment_.AdvanceClock(base::Days(3));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 5);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 1, 2);

  task_environment_.AdvanceClock(base::Days(2));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklySessionCountHistogramName, 6);
  histogram_tester_.ExpectBucketCount(kWeeklySessionCountHistogramName, 0, 1);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklySessionCount), 0);
}

TEST_F(HnsNewsP3ATest, TestWeeklyDisplayAdsViewedCount) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklyDisplayAdsViewedHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kWeeklyDisplayAdsViewedHistogramName, 0,
                                      1);

  RecordWeeklyDisplayAdsViewedCount(prefs, true);
  RecordWeeklyDisplayAdsViewedCount(prefs, true);

  task_environment_.AdvanceClock(base::Days(2));
  RecordWeeklyDisplayAdsViewedCount(prefs, true);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklyDisplayAdViewedCount), 3);

  task_environment_.AdvanceClock(base::Days(2));
  RecordWeeklyDisplayAdsViewedCount(prefs, false);
  histogram_tester_.ExpectTotalCount(kWeeklyDisplayAdsViewedHistogramName, 5);
  histogram_tester_.ExpectBucketCount(kWeeklyDisplayAdsViewedHistogramName, 2,
                                      3);

  task_environment_.AdvanceClock(base::Days(3));
  RecordWeeklyDisplayAdsViewedCount(prefs, false);
  histogram_tester_.ExpectTotalCount(kWeeklyDisplayAdsViewedHistogramName, 6);
  histogram_tester_.ExpectBucketCount(kWeeklyDisplayAdsViewedHistogramName, 1,
                                      2);

  task_environment_.AdvanceClock(base::Days(2));
  RecordWeeklyDisplayAdsViewedCount(prefs, false);
  histogram_tester_.ExpectTotalCount(kWeeklyDisplayAdsViewedHistogramName, 7);
  histogram_tester_.ExpectBucketCount(kWeeklyDisplayAdsViewedHistogramName, 0,
                                      2);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklyDisplayAdViewedCount), 0);
}

TEST_F(HnsNewsP3ATest, TestWeeklyAddedDirectFeedsCount) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kWeeklyAddedDirectFeedsHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kWeeklyAddedDirectFeedsHistogramName, 0,
                                      1);

  RecordWeeklyAddedDirectFeedsCount(prefs, 1);
  RecordWeeklyAddedDirectFeedsCount(prefs, 1);

  task_environment_.AdvanceClock(base::Days(2));
  RecordWeeklyAddedDirectFeedsCount(prefs, 0);
  histogram_tester_.ExpectTotalCount(kWeeklyAddedDirectFeedsHistogramName, 4);
  histogram_tester_.ExpectBucketCount(kWeeklyAddedDirectFeedsHistogramName, 2,
                                      2);

  RecordWeeklyAddedDirectFeedsCount(prefs, 1);
  RecordWeeklyAddedDirectFeedsCount(prefs, 1);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklyAddedDirectFeedsCount), 4);

  histogram_tester_.ExpectTotalCount(kWeeklyAddedDirectFeedsHistogramName, 6);
  histogram_tester_.ExpectBucketCount(kWeeklyAddedDirectFeedsHistogramName, 4,
                                      1);
  RecordWeeklyAddedDirectFeedsCount(prefs, -1);
  histogram_tester_.ExpectTotalCount(kWeeklyAddedDirectFeedsHistogramName, 7);
  histogram_tester_.ExpectBucketCount(kWeeklyAddedDirectFeedsHistogramName, 3,
                                      2);

  task_environment_.AdvanceClock(base::Days(6));
  RecordWeeklyAddedDirectFeedsCount(prefs, 0);
  histogram_tester_.ExpectTotalCount(kWeeklyAddedDirectFeedsHistogramName, 8);
  histogram_tester_.ExpectBucketCount(kWeeklyAddedDirectFeedsHistogramName, 1,
                                      2);

  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsWeeklyAddedDirectFeedsCount), 1);
}

TEST_F(HnsNewsP3ATest, TestDirectFeedsTotal) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kDirectFeedsTotalHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kDirectFeedsTotalHistogramName, 0, 1);

  ScopedDictPrefUpdate update1(prefs, prefs::kHnsNewsDirectFeeds);
  update1->Set("id1", base::Value::Dict());
  ScopedDictPrefUpdate update2(prefs, prefs::kHnsNewsDirectFeeds);
  update2->Set("id2", base::Value::Dict());

  RecordDirectFeedsTotal(prefs);
  histogram_tester_.ExpectTotalCount(kDirectFeedsTotalHistogramName, 2);
  histogram_tester_.ExpectBucketCount(kDirectFeedsTotalHistogramName, 2, 1);
}

TEST_F(HnsNewsP3ATest, TestTotalCardsViewed) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kTotalCardViewsHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 0, 1);

  RecordTotalCardViews(prefs, 0);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 0, 2);

  RecordTotalCardViews(prefs, 1);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 1, 1);

  RecordTotalCardViews(prefs, 14);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 3, 1);

  task_environment_.AdvanceClock(base::Days(4));
  EXPECT_EQ(GetWeeklySum(prefs::kHnsNewsTotalCardViews), 15);

  RecordAtSessionStart(prefs);
  RecordTotalCardViews(prefs, 15);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 4, 1);

  RecordAtSessionStart(prefs);
  RecordTotalCardViews(prefs, 15);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 5, 1);

  task_environment_.AdvanceClock(base::Days(4));

  RecordAtSessionStart(prefs);
  RecordTotalCardViews(prefs, 0);
  histogram_tester_.ExpectBucketCount(kTotalCardViewsHistogramName, 4, 2);
}

TEST_F(HnsNewsP3ATest, TestLastUsageTime) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  // Should not report if News was never used
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 0);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 1, 1);

  task_environment_.AdvanceClock(base::Days(7));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 2);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 2, 1);

  task_environment_.AdvanceClock(base::Days(7));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 3);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 3, 1);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 4);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 1, 2);

  task_environment_.AdvanceClock(base::Days(21));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 5);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 4, 1);

  task_environment_.AdvanceClock(base::Days(7));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 6);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 5, 1);

  task_environment_.AdvanceClock(base::Days(33));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 7);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 6, 1);

  task_environment_.AdvanceClock(base::Days(90));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kLastUsageTimeHistogramName, 8);
  histogram_tester_.ExpectBucketCount(kLastUsageTimeHistogramName, 6, 2);
}

TEST_F(HnsNewsP3ATest, TestNewUserReturningFollowingDay) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 0, 1);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 2);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 2, 1);

  task_environment_.AdvanceClock(base::Days(1));
  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 3);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 3, 1);

  task_environment_.AdvanceClock(base::Days(2));
  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 4);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 3, 2);

  task_environment_.AdvanceClock(base::Days(5));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 5);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 1, 1);
}

TEST_F(HnsNewsP3ATest, TestNewUserReturningNotFollowingDay) {
  PrefService* prefs = GetPrefs();
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 1);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 0, 1);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 2);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 2, 1);

  task_environment_.AdvanceClock(base::Days(2));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 3);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 2, 2);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 4);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 4, 1);

  task_environment_.AdvanceClock(base::Days(2));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 5);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 4, 2);

  task_environment_.AdvanceClock(base::Days(4));
  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kNewUserReturningHistogramName, 6);
  histogram_tester_.ExpectBucketCount(kNewUserReturningHistogramName, 1, 1);
}

TEST_F(HnsNewsP3ATest, TestIsEnabled) {
  PrefService* prefs = GetPrefs();

  // should not record "disabled" if never opted in
  prefs->SetBoolean(prefs::kNewTabPageShowToday, false);
  histogram_tester_.ExpectTotalCount(kIsEnabledHistogramName, 0);

  prefs->SetBoolean(prefs::kHnsNewsOptedIn, true);
  prefs->SetBoolean(prefs::kNewTabPageShowToday, true);
  RecordFeatureEnabledChange(prefs);
  histogram_tester_.ExpectUniqueSample(kIsEnabledHistogramName, 1, 1);

  prefs->SetBoolean(prefs::kNewTabPageShowToday, false);
  RecordFeatureEnabledChange(prefs);
  histogram_tester_.ExpectBucketCount(kIsEnabledHistogramName, 0, 1);
}

TEST_F(HnsNewsP3ATest, TestGeneralUsage) {
  PrefService* prefs = GetPrefs();

  RecordAtInit(prefs);
  histogram_tester_.ExpectTotalCount(kUsageDailyHistogramName, 0);
  histogram_tester_.ExpectTotalCount(kUsageMonthlyHistogramName, 0);

  RecordAtSessionStart(prefs);
  histogram_tester_.ExpectUniqueSample(kUsageDailyHistogramName, 1, 1);
  histogram_tester_.ExpectUniqueSample(kUsageMonthlyHistogramName, 1, 1);
}

}  // namespace p3a
}  // namespace hns_news
