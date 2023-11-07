/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_HNS_STATS_HELPER_H_
#define HNS_BROWSER_HNS_ADS_HNS_STATS_HELPER_H_

#include "base/scoped_observation.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_manager_observer.h"
#include "chrome/browser/profiles/profile_observer.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"

class PrefRegistrySimple;
class Profile;

namespace hns_ads {

extern const char kAdsEnabledInstallationTimeHistogramName[];

class HnsStatsHelper : public ProfileManagerObserver, public ProfileObserver {
 public:
  HnsStatsHelper();
  ~HnsStatsHelper() override;

  static void RegisterLocalStatePrefs(PrefRegistrySimple* registry);

  void OnProfileAdded(Profile* profile) override;
  void OnProfileManagerDestroying() override;

  void OnProfileWillBeDestroyed(Profile* profile) override;

  void SetFirstRunTimeForTesting(base::Time time);

 private:
  PrefService* GetLastUsedProfilePrefs();
  void OnLastUsedProfileChanged();

  void Update();
  void UpdateLocalStateAdsEnabled(bool is_enabled_for_current_profile);
  void MaybeReportAdsInstallationTimeMetric(
      bool is_enabled_for_current_profile);

#if !BUILDFLAG(IS_ANDROID)
  PrefChangeRegistrar last_used_profile_pref_change_registrar_;
#endif
  PrefChangeRegistrar ads_enabled_pref_change_registrar_;
  raw_ptr<Profile> current_profile_ = nullptr;

  base::ScopedObservation<ProfileManager, ProfileManagerObserver>
      profile_manager_observer_{this};

  raw_ptr<PrefService> local_state_;
  raw_ptr<ProfileManager> profile_manager_;

  base::Time testing_first_run_time_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_HNS_STATS_HELPER_H_
