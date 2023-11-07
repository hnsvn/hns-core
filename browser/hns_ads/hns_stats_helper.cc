/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_ads/hns_stats_helper.h"

#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/time/time.h"
#include "hns/browser/hns_stats/first_run_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/p3a_utils/bucket.h"
#include "chrome/browser/browser_process.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_registry_simple.h"

namespace hns_ads {

namespace {

const int kAdsEnabledInstallationHourBuckets[] = {0, 11, 23, 71};

}  // namespace

const char kAdsEnabledInstallationTimeHistogramName[] =
    "Hns.Rewards.EnabledInstallationTime";

HnsStatsHelper::HnsStatsHelper()
    : local_state_(g_browser_process->local_state()),
      profile_manager_(g_browser_process->profile_manager()) {
#if !BUILDFLAG(IS_ANDROID)
  last_used_profile_pref_change_registrar_.Init(local_state_);
  last_used_profile_pref_change_registrar_.Add(
      ::prefs::kProfileLastUsed,
      base::BindRepeating(&HnsStatsHelper::OnLastUsedProfileChanged,
                          base::Unretained(this)));
#endif

  profile_manager_observer_.Observe(profile_manager_);
}

HnsStatsHelper::~HnsStatsHelper() {
  if (current_profile_) {
    current_profile_->RemoveObserver(this);
  }
}

void HnsStatsHelper::RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(prefs::kEnabledForLastProfile, false);
  registry->RegisterBooleanPref(prefs::kEverEnabledForAnyProfile, false);
}

void HnsStatsHelper::OnProfileAdded(Profile* profile) {
#if BUILDFLAG(IS_ANDROID)
  if (profile == ProfileManager::GetPrimaryUserProfile()) {
#else
  base::FilePath last_used_path =
      local_state_->GetFilePath(::prefs::kProfileLastUsed);
  if ((!last_used_path.empty() && profile->GetBaseName() == last_used_path) ||
      (last_used_path.empty() &&
       profile == ProfileManager::GetLastUsedProfile())) {
#endif
    OnLastUsedProfileChanged();
  }
}

void HnsStatsHelper::OnProfileWillBeDestroyed(Profile* profile) {
  if (profile != current_profile_) {
    return;
  }
  profile->RemoveObserver(this);
  current_profile_ = nullptr;
#if !BUILDFLAG(IS_ANDROID)
  last_used_profile_pref_change_registrar_.RemoveAll();
#endif
  ads_enabled_pref_change_registrar_.RemoveAll();
}

void HnsStatsHelper::OnProfileManagerDestroying() {
  if (current_profile_ != nullptr) {
#if !BUILDFLAG(IS_ANDROID)
    last_used_profile_pref_change_registrar_.RemoveAll();
#endif
    ads_enabled_pref_change_registrar_.RemoveAll();
    current_profile_->RemoveObserver(this);
    current_profile_ = nullptr;
  }
  profile_manager_observer_.Reset();
}

void HnsStatsHelper::SetFirstRunTimeForTesting(base::Time time) {
  testing_first_run_time_ = time;
}

PrefService* HnsStatsHelper::GetLastUsedProfilePrefs() {
#if BUILDFLAG(IS_ANDROID)
  return ProfileManager::GetPrimaryUserProfile()->GetPrefs();
#else

  base::FilePath last_used_profile_path =
      local_state_->GetFilePath(::prefs::kProfileLastUsed);
  Profile* profile;
  if (last_used_profile_path.empty()) {
    profile = profile_manager_->GetLastUsedProfile();
  } else {
    profile = profile_manager_->GetProfileByPath(
        profile_manager_->user_data_dir().Append(last_used_profile_path));
  }
  if (profile == nullptr || profile->IsOffTheRecord()) {
    return nullptr;
  }
  if (current_profile_ != nullptr) {
    current_profile_->RemoveObserver(this);
    current_profile_ = nullptr;
  }
  current_profile_ = profile;
  profile->AddObserver(this);
  return profile->GetPrefs();
#endif
}

void HnsStatsHelper::OnLastUsedProfileChanged() {
  PrefService* profile_prefs = GetLastUsedProfilePrefs();
  if (profile_prefs == nullptr) {
    return;
  }
  ads_enabled_pref_change_registrar_.RemoveAll();
  ads_enabled_pref_change_registrar_.Init(profile_prefs);
  ads_enabled_pref_change_registrar_.Add(
      prefs::kOptedInToNotificationAds,
      base::BindRepeating(&HnsStatsHelper::Update, base::Unretained(this)));
  Update();
}

void HnsStatsHelper::Update() {
  PrefService* profile_prefs = GetLastUsedProfilePrefs();
  if (profile_prefs == nullptr) {
    return;
  }
  bool is_enabled = profile_prefs->GetBoolean(prefs::kOptedInToNotificationAds);
  UpdateLocalStateAdsEnabled(is_enabled);
  MaybeReportAdsInstallationTimeMetric(is_enabled);
}

void HnsStatsHelper::UpdateLocalStateAdsEnabled(
    bool is_enabled_for_current_profile) {
  // Copying enabled pref to local state so the stats updater does not depend on
  // the profile
  local_state_->SetBoolean(prefs::kEnabledForLastProfile,
                           is_enabled_for_current_profile);
}

void HnsStatsHelper::MaybeReportAdsInstallationTimeMetric(
    bool is_enabled_for_current_profile) {
  if (!is_enabled_for_current_profile ||
      local_state_->GetBoolean(prefs::kEverEnabledForAnyProfile)) {
    // If ads was already enabled for a previous profile or the current profile,
    // assume the metric was already sent.
    return;
  }
  local_state_->SetBoolean(prefs::kEverEnabledForAnyProfile, true);

  base::Time first_run = !testing_first_run_time_.is_null()
                             ? testing_first_run_time_
                             : hns_stats::GetFirstRunTime(local_state_);
  int hours_from_first_run = (base::Time::Now() - first_run).InHours();

  p3a_utils::RecordToHistogramBucket(kAdsEnabledInstallationTimeHistogramName,
                                     kAdsEnabledInstallationHourBuckets,
                                     hours_from_first_run);
}

}  // namespace hns_ads
