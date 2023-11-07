// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/profiles/hns_profile_manager.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/metrics/histogram_macros.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_federated/hns_federated_service_factory.h"
#include "hns/browser/hns_news/hns_news_controller_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/perf/hns_perf_features_processor.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/url_sanitizer/url_sanitizer_service_factory.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/content_settings/core/browser/hns_content_settings_pref_provider.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/profiles/profile_attributes_entry.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profiles_state.h"
#include "chrome/common/chrome_constants.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/generated_resources.h"
#include "components/bookmarks/common/bookmark_pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"
#include "components/signin/public/base/signin_pref_names.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/url_data_source.h"
#include "ui/base/l10n/l10n_util.h"

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "hns/browser/gcm_driver/hns_gcm_channel_status.h"
#endif

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
#include "hns/browser/hns_wallet/hns_wallet_auto_pin_service_factory.h"
#endif  // BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/components/ipfs/ipfs_service.h"
#endif  // BUILDFLAG(ENABLE_IPFS)

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/tor_constants.h"
#endif

using content::BrowserThread;

HnsProfileManager::HnsProfileManager(const base::FilePath& user_data_dir)
    : ProfileManager(user_data_dir) {
  MigrateProfileNames();
}

void HnsProfileManager::InitProfileUserPrefs(Profile* profile) {
  // migrate obsolete plugin prefs to temporary migration pref because otherwise
  // they get deleteed by PrefProvider before we can migrate them in
  // HnsPrefProvider
  content_settings::HnsPrefProvider::CopyPluginSettingsForMigration(
      profile->GetPrefs());

// Chromecast is enabled by default on Android.
#if !BUILDFLAG(IS_ANDROID)
  auto* pref_service = profile->GetPrefs();
  // At start, the value of kEnableMediaRouterOnRestart is updated to match
  // kEnableMediaRouter so users don't lose their current setting
  if (pref_service->FindPreference(kEnableMediaRouterOnRestart)
          ->IsDefaultValue()) {
    auto enabled = pref_service->GetBoolean(::prefs::kEnableMediaRouter);
    pref_service->SetBoolean(kEnableMediaRouterOnRestart, enabled);
  } else {
    // For Desktop, kEnableMediaRouterOnRestart is used to track the current
    // state of the media router switch in hns://settings/extensions. The
    // value of kEnableMediaRouter is only updated to match
    // kEnableMediaRouterOnRestart on restart
    auto enabled = pref_service->GetBoolean(kEnableMediaRouterOnRestart);
    pref_service->SetBoolean(::prefs::kEnableMediaRouter, enabled);
  }
#endif

  ProfileManager::InitProfileUserPrefs(profile);
  hns::RecordInitialP3AValues(profile);
  hns::SetDefaultSearchVersion(profile, profile->IsNewProfile());
  hns::SetDefaultThirdPartyCookieBlockValue(profile);
  perf::MaybeEnableHnsFeatureForPerfTesting(profile);
  hns::MigrateHttpsUpgradeSettings(profile);
#if BUILDFLAG(ENABLE_IPFS)
  ipfs::IpfsService::MigrateProfilePrefs(profile->GetPrefs());
#endif
}

void HnsProfileManager::DoFinalInitForServices(Profile* profile,
                                                 bool go_off_the_record) {
  ProfileManager::DoFinalInitForServices(profile, go_off_the_record);
  if (!do_final_services_init_)
    return;
  hns_ads::AdsServiceFactory::GetForProfile(profile);
  hns_rewards::RewardsServiceFactory::GetForProfile(profile);
#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
  hns_wallet::HnsWalletAutoPinServiceFactory::GetServiceForContext(profile);
#endif  // BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
#if BUILDFLAG(ENABLE_IPFS)
  ipfs::IpfsServiceFactory::GetForContext(profile);
#endif  // BUILDFLAG(ENABLE_IPFS)
  hns_wallet::HnsWalletServiceFactory::GetServiceForContext(profile);
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  gcm::HnsGCMChannelStatus* status =
      gcm::HnsGCMChannelStatus::GetForProfile(profile);
  DCHECK(status);
  status->UpdateGCMDriverStatus();
#endif
  hns_news::HnsNewsControllerFactory::GetForContext(profile);
  hns_federated::HnsFederatedServiceFactory::GetForBrowserContext(profile);
  hns::URLSanitizerServiceFactory::GetForBrowserContext(profile);
}

bool HnsProfileManager::IsAllowedProfilePath(
    const base::FilePath& path) const {
  // Chromium only allows profiles to be created in the user_data_dir, but we
  // want to also be able to create profile in subfolders of user_data_dir.
  return ProfileManager::IsAllowedProfilePath(path) ||
         user_data_dir().IsParent(path.DirName());
}

bool HnsProfileManager::LoadProfileByPath(const base::FilePath& profile_path,
                                            bool incognito,
                                            ProfileLoadedCallback callback) {
#if BUILDFLAG(ENABLE_TOR)
  // Prevent legacy tor session profile to be loaded so we won't hit
  // DCHECK(!GetProfileAttributesWithPath(...)). Workaround for legacy tor guest
  // profile won't work because when AddProfile to storage we will hit
  // DCHECK(user_data_dir_ == profile_path.DirName()), legacy tor session
  // profile was not under user_data_dir like legacy tor guest profile did.
  if (profile_path.BaseName().value() == tor::kTorProfileDir) {
    return false;
  }
#endif
  return ProfileManager::LoadProfileByPath(profile_path, incognito,
                                           std::move(callback));
}

// This overridden method doesn't clear |kDefaultSearchProviderDataPrefName|.
// W/o this, prefs set by TorWindowSearchEngineProviderService is cleared
// during the initialization.
void HnsProfileManager::SetNonPersonalProfilePrefs(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  prefs->SetBoolean(prefs::kSigninAllowed, false);
  prefs->SetBoolean(bookmarks::prefs::kEditBookmarksEnabled, false);
  prefs->SetBoolean(bookmarks::prefs::kShowBookmarkBar, false);
}

void HnsProfileManager::MigrateProfileNames() {
#if !BUILDFLAG(IS_ANDROID) && !BUILDFLAG(IS_CHROMEOS)
  // If any profiles have a default name using an
  // older version of the default name string format,
  // then name it with the new default name string format.
  // e.g. 'Person X' --> 'Profile X'.
  ProfileAttributesStorage& storage = GetProfileAttributesStorage();
  std::vector<ProfileAttributesEntry*> entries =
      storage.GetAllProfilesAttributesSortedByNameWithCheck();
  // Make sure we keep the numbering the same.
  for (auto* entry : entries) {
    // Rename the necessary profiles. Don't check for legacy names as profile
    // info cache should have migrated them by now.
    if (entry->IsUsingDefaultName() &&
        !storage.IsDefaultProfileName(
            entry->GetName(),
            /*include_check_for_legacy_profile_name=*/false)) {
      auto icon_index = entry->GetAvatarIconIndex();
      entry->SetLocalProfileName(storage.ChooseNameForNewProfile(icon_index),
                                 /*is_default_name=*/true);
    }
  }
#endif
}

HnsProfileManagerWithoutInit::HnsProfileManagerWithoutInit(
    const base::FilePath& user_data_dir)
    : HnsProfileManager(user_data_dir) {
  set_do_final_services_init(false);
}
