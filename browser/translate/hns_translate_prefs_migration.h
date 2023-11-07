/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_TRANSLATE_HNS_TRANSLATE_PREFS_MIGRATION_H_
#define HNS_BROWSER_TRANSLATE_HNS_TRANSLATE_PREFS_MIGRATION_H_

class PrefRegistrySimple;
class PrefService;

namespace translate {

namespace prefs {
extern const char kMigratedToInternalTranslation[];
}  // namespace prefs

void RegisterHnsProfilePrefsForMigration(PrefRegistrySimple* registry);

void ClearMigrationHnsProfilePrefs(PrefService* prefs);

}  // namespace translate

#endif  // HNS_BROWSER_TRANSLATE_HNS_TRANSLATE_PREFS_MIGRATION_H_
