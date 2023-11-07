/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/hns_wallet/browser/hns_wallet_prefs.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/decentralized_dns/core/utils.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "hns/components/p3a/buildflags.h"
#include "hns/components/p3a/p3a_service.h"
#include "hns/ios/browser/hns_stats/hns_stats_prefs.h"
#include "components/pref_registry/pref_registry_syncable.h"

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/ipfs_service.h"
#endif

void HnsRegisterBrowserStatePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  hns_sync::Prefs::RegisterProfilePrefs(registry);
  hns_wallet::RegisterProfilePrefs(registry);
  hns_wallet::RegisterProfilePrefsForMigration(registry);
#if BUILDFLAG(ENABLE_IPFS)
  ipfs::IpfsService::RegisterProfilePrefs(registry);
#endif
}

void HnsRegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  hns_stats::RegisterLocalStatePrefs(registry);
  hns_wallet::RegisterLocalStatePrefs(registry);
  hns_wallet::RegisterLocalStatePrefsForMigration(registry);
  decentralized_dns::RegisterLocalStatePrefs(registry);
#if BUILDFLAG(HNS_P3A_ENABLED)
  p3a::P3AService::RegisterPrefs(registry, false);
#endif
  ntp_background_images::NTPBackgroundImagesService::RegisterLocalStatePrefs(
      registry);
}

void HnsMigrateObsoleteBrowserStatePrefs(PrefService* prefs) {
  hns_wallet::KeyringService::MigrateObsoleteProfilePrefs(prefs);
  hns_wallet::MigrateObsoleteProfilePrefs(prefs);
}

#define HNS_REGISTER_BROWSER_STATE_PREFS \
  HnsRegisterBrowserStatePrefs(registry);
#define HNS_REGISTER_LOCAL_STATE_PREFS HnsRegisterLocalStatePrefs(registry);
#define HNS_MIGRATE_OBSOLETE_BROWSER_STATE_PREFS \
  HnsMigrateObsoleteBrowserStatePrefs(prefs);
#include "src/ios/chrome/browser/shared/model/prefs/browser_prefs.mm"
