/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_STATS_HNS_STATS_UPDATER_PARAMS_H_
#define HNS_BROWSER_HNS_STATS_HNS_STATS_UPDATER_PARAMS_H_

#include <cstdint>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "base/time/time.h"
#include "hns/components/hns_stats/browser/hns_stats_updater_util.h"

class HnsStatsUpdaterTest;
class GURL;
class PrefService;

FORWARD_DECLARE_TEST(HnsStatsUpdaterTest, UsageBitstringDaily);
FORWARD_DECLARE_TEST(HnsStatsUpdaterTest, UsageBitstringWeekly);
FORWARD_DECLARE_TEST(HnsStatsUpdaterTest, UsageBitstringMonthly);
FORWARD_DECLARE_TEST(HnsStatsUpdaterTest, UsageBitstringInactive);

namespace hns_stats {

class HnsStatsUpdaterParams {
 public:
  explicit HnsStatsUpdaterParams(PrefService* stats_pref_service,
                                   const ProcessArch arch);
  HnsStatsUpdaterParams(PrefService* stats_pref_service,
                          const ProcessArch arch,
                          const std::string& ymd,
                          int woy,
                          int month);
  HnsStatsUpdaterParams(const HnsStatsUpdaterParams&) = delete;
  HnsStatsUpdaterParams& operator=(const HnsStatsUpdaterParams&) = delete;
  ~HnsStatsUpdaterParams();

  std::string GetDailyParam() const;
  std::string GetWeeklyParam() const;
  std::string GetMonthlyParam() const;
  std::string GetFirstCheckMadeParam() const;
  std::string GetWeekOfInstallationParam() const;
  std::string GetDateOfInstallationParam() const;
  std::string GetReferralCodeParam() const;
  std::string GetAdsEnabledParam() const;
  std::string GetProcessArchParam() const;
  std::string GetWalletEnabledParam() const;
  GURL GetUpdateURL(const GURL& base_update_url,
                    const std::string platform_id,
                    const std::string channel_name,
                    const std::string full_hns_version) const;

  void SavePrefs();

 private:
  friend class ::HnsStatsUpdaterTest;
  FRIEND_TEST_ALL_PREFIXES(::HnsStatsUpdaterTest, UsageBitstringDaily);
  FRIEND_TEST_ALL_PREFIXES(::HnsStatsUpdaterTest, UsageBitstringWeekly);
  FRIEND_TEST_ALL_PREFIXES(::HnsStatsUpdaterTest, UsageBitstringMonthly);
  FRIEND_TEST_ALL_PREFIXES(::HnsStatsUpdaterTest, UsageBitstringInactive);

  raw_ptr<PrefService> stats_pref_service_ = nullptr;
  ProcessArch arch_;
  std::string ymd_;
  int woy_;
  int month_;
  std::string last_check_ymd_;
  int last_check_woy_;
  int last_check_month_;
  bool first_check_made_;
  std::string week_of_installation_;
  base::Time date_of_installation_;
  base::Time wallet_last_unlocked_;
  base::Time last_reported_wallet_unlock_;
  std::string referral_promo_code_;
  static base::Time g_current_time;
  static bool g_force_first_run;

  void LoadPrefs();

  std::string BooleanToString(bool bool_value) const;

  std::string GetDateAsYMD(const base::Time& time) const;
  std::string GetCurrentDateAsYMD() const;
  std::string GetLastMondayAsYMD() const;
  int GetCurrentMonth() const;
  int GetCurrentISOWeekNumber() const;

  // Returns the current time, allows override from tests
  base::Time GetCurrentTimeNow() const;

  // Gets the previous day, since that is the most recent time another stats
  // ping could have fired.
  base::Time GetReferenceTime() const;

  bool ShouldForceFirstRun() const;

  static void SetCurrentTimeForTest(const base::Time& current_time);
  static void SetFirstRunForTest(bool first_run);
};

}  // namespace hns_stats

#endif  // HNS_BROWSER_HNS_STATS_HNS_STATS_UPDATER_PARAMS_H_
