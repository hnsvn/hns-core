/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/translate/hns_translate_prefs_migration.h"

#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/translate/core/browser/translate_pref_names.h"

namespace translate {

namespace prefs {
const char kMigratedToInternalTranslation[] =
    "hns.translate_migrated_from_extension";
}  // namespace prefs

void RegisterHnsProfilePrefsForMigration(PrefRegistrySimple* registry) {
  // No sync for migration prefs
  registry->RegisterBooleanPref(prefs::kMigratedToInternalTranslation, false);
}

void ClearMigrationHnsProfilePrefs(PrefService* prefs) {
  prefs->ClearPref(prefs::kMigratedToInternalTranslation);
}

}  // namespace translate
