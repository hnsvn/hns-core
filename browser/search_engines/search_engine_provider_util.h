/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_SEARCH_ENGINES_SEARCH_ENGINE_PROVIDER_UTIL_H_
#define HNS_BROWSER_SEARCH_ENGINES_SEARCH_ENGINE_PROVIDER_UTIL_H_

class Profile;
class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace hns {

bool IsRegionForQwant(Profile* profile);
void SetHnsAsDefaultPrivateSearchProvider(PrefService* prefs);

// For prefs migration.
void RegisterSearchEngineProviderPrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry);
void MigrateSearchEngineProviderPrefs(Profile* profile);

// Initialize default provider for private profile.
void PrepareDefaultPrivateSearchProviderDataIfNeeded(Profile* profile);

// Update TemplareURLData with provider guid.
void UpdateDefaultPrivateSearchProviderData(Profile* profile);
void ResetDefaultPrivateSearchProvider(Profile* profile);

}  // namespace hns

#endif  // HNS_BROWSER_SEARCH_ENGINES_SEARCH_ENGINE_PROVIDER_UTIL_H_
