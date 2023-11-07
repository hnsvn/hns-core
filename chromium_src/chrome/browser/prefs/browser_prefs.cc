/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_local_state_prefs.h"
#include "hns/browser/hns_profile_prefs.h"
#include "hns/browser/hns_rewards/rewards_prefs_util.h"
#include "hns/browser/search/ntp_utils.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/browser/translate/hns_translate_prefs_migration.h"
#include "hns/components/hns_news/browser/hns_news_p3a.h"
#include "hns/components/hns_search_conversion/p3a.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/browser/hns_wallet_prefs.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/decentralized_dns/core/utils.h"
#include "hns/components/ntp_background_images/buildflags/buildflags.h"
#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/translate/core/browser/translate_prefs.h"
#include "extensions/buildflags/buildflags.h"
#include "third_party/widevine/cdm/buildflags.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/p3a/p3a_core_metrics.h"
#include "hns/browser/search_engines/search_engine_provider_util.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/tor_utils.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "hns/browser/widevine/widevine_utils.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#endif

#if !BUILDFLAG(ENABLE_EXTENSIONS)
#define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_PROVIDER_H_
#endif  // !BUILDFLAG(ENABLE_EXTENSIONS)

#define MigrateObsoleteProfilePrefs MigrateObsoleteProfilePrefs_ChromiumImpl
#define MigrateObsoleteLocalStatePrefs \
  MigrateObsoleteLocalStatePrefs_ChromiumImpl
#include "src/chrome/browser/prefs/browser_prefs.cc"
#undef MigrateObsoleteProfilePrefs
#undef MigrateObsoleteLocalStatePrefs

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "hns/browser/gcm_driver/hns_gcm_utils.h"
#endif

#if BUILDFLAG(ENABLE_CUSTOM_BACKGROUND)
#include "hns/browser/ntp_background/ntp_background_prefs.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "hns/components/sidebar/pref_names.h"
#endif

// This method should be periodically pruned of year+ old migrations.
void MigrateObsoleteProfilePrefs(Profile* profile) {
  // BEGIN_MIGRATE_OBSOLETE_PROFILE_PREFS
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Added 02/2020.
  // Must be called before ChromiumImpl because it's migrating a Chromium pref
  // to Hns pref.
  gcm::MigrateGCMPrefs(profile);
#endif

  MigrateObsoleteProfilePrefs_ChromiumImpl(profile);

#if BUILDFLAG(ENABLE_WIDEVINE)
  // Added 11/2019.
  MigrateWidevinePrefs(profile);
#endif
  hns_sync::MigrateHnsSyncPrefs(profile->GetPrefs());

  // Added 12/2019.
  dark_mode::MigrateHnsDarkModePrefs(profile);

#if !BUILDFLAG(IS_ANDROID)
  // Added 9/2020
  new_tab_page::MigrateNewTabPagePrefs(profile);

  // Added 06/2022
  hns::MigrateSearchEngineProviderPrefs(profile);

  // Added 10/2022
  profile->GetPrefs()->ClearPref(kDefaultBrowserLaunchingCount);
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
  // Added 11/2022
  profile->GetPrefs()->ClearPref(kDontAskEnableWebDiscovery);
  profile->GetPrefs()->ClearPref(kHnsSearchVisitCount);
#endif

  hns_wallet::KeyringService::MigrateObsoleteProfilePrefs(
      profile->GetPrefs());
  hns_wallet::MigrateObsoleteProfilePrefs(profile->GetPrefs());

  // Added 05/2021
  profile->GetPrefs()->ClearPref(kHnsNewsIntroDismissed);
  // Added 07/2021
  profile->GetPrefs()->ClearPref(prefs::kNetworkPredictionOptions);

  // Added 01/2022
  hns_rewards::MigrateObsoleteProfilePrefs(profile->GetPrefs());

  // Added 05/2022
  translate::ClearMigrationHnsProfilePrefs(profile->GetPrefs());

  // Added 06/2022
#if BUILDFLAG(ENABLE_CUSTOM_BACKGROUND)
  NTPBackgroundPrefs(profile->GetPrefs()).MigrateOldPref();
#endif

  // Added 24/11/2022: https://github.com/hnsvn/hns-core/pull/16027
#if !BUILDFLAG(IS_IOS) && !BUILDFLAG(IS_ANDROID)
  profile->GetPrefs()->ClearPref(kFTXAccessToken);
  profile->GetPrefs()->ClearPref(kFTXOauthHost);
  profile->GetPrefs()->ClearPref(kFTXNewTabPageShowFTX);
  profile->GetPrefs()->ClearPref(kCryptoDotComNewTabPageShowCryptoDotCom);
  profile->GetPrefs()->ClearPref(kCryptoDotComHasBoughtCrypto);
  profile->GetPrefs()->ClearPref(kCryptoDotComHasInteracted);
  profile->GetPrefs()->ClearPref(kGeminiAccessToken);
  profile->GetPrefs()->ClearPref(kGeminiRefreshToken);
  profile->GetPrefs()->ClearPref(kNewTabPageShowGemini);
#endif

  // Added 24/11/2022: https://github.com/hnsvn/hns-core/pull/16027
#if !BUILDFLAG(IS_IOS)
  profile->GetPrefs()->ClearPref(kBinanceAccessToken);
  profile->GetPrefs()->ClearPref(kBinanceRefreshToken);
  profile->GetPrefs()->ClearPref(kNewTabPageShowBinance);
  profile->GetPrefs()->ClearPref(kHnsSuggestedSiteSuggestionsEnabled);
#endif

#if defined(TOOLKIT_VIEWS)
  // Added May 2023
  if (profile->GetPrefs()->GetBoolean(
          sidebar::kSidebarAlignmentChangedTemporarily)) {
    // If temporarily changed, it means sidebar is set to right.
    // Just clear alignment prefs as default alignment is changed to right.
    profile->GetPrefs()->ClearPref(prefs::kSidePanelHorizontalAlignment);
  }

  profile->GetPrefs()->ClearPref(sidebar::kSidebarAlignmentChangedTemporarily);
#endif

  hns_news::p3a::MigrateObsoleteProfilePrefs(profile->GetPrefs());

  // END_MIGRATE_OBSOLETE_PROFILE_PREFS
}

// This method should be periodically pruned of year+ old migrations.
void MigrateObsoleteLocalStatePrefs(PrefService* local_state) {
  // BEGIN_MIGRATE_OBSOLETE_LOCAL_STATE_PREFS
  MigrateObsoleteLocalStatePrefs_ChromiumImpl(local_state);

#if BUILDFLAG(ENABLE_WIDEVINE)
  // Added 11/2020.
  MigrateObsoleteWidevineLocalStatePrefs(local_state);
#endif

#if BUILDFLAG(ENABLE_TOR)
  // Added 4/2021.
  tor::MigrateLastUsedProfileFromLocalStatePrefs(local_state);
#endif

  decentralized_dns::MigrateObsoleteLocalStatePrefs(local_state);

#if BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)
  // Migrating the feature flag here because dependencies relying on its value.
  hns_vpn::MigrateWireguardFeatureFlag(local_state);
#endif

#if !BUILDFLAG(IS_ANDROID)
  // Added 10/2022
  local_state->ClearPref(kDefaultBrowserPromptEnabled);
  hns::HnsUptimeTracker::MigrateObsoletePrefs(local_state);
#endif

  hns_search_conversion::p3a::MigrateObsoleteLocalStatePrefs(local_state);

  // END_MIGRATE_OBSOLETE_LOCAL_STATE_PREFS
}
