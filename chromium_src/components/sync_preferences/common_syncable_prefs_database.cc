/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/sync_preferences/common_syncable_prefs_database.h"

#include "base/containers/fixed_flat_map.h"
#include "base/strings/string_piece.h"
#include "components/search_engines/search_engines_pref_names.h"
// "//components/sync_preferences:common_syncable_prefs_database" already
// depends on "//components/search_engines"

namespace sync_preferences {
namespace {
namespace hns_syncable_prefs_ids {
enum {
  kSyncedDefaultPrivateSearchProviderGUID = 1000,
  kSyncedDefaultPrivateSearchProviderData = 1001
};
}  // namespace hns_syncable_prefs_ids

const auto& HnsSyncablePreferences() {
  static const auto kHnsCommonSyncablePrefsAllowlist =
      base::MakeFixedFlatMap<base::StringPiece, SyncablePrefMetadata>(
          {{prefs::kSyncedDefaultPrivateSearchProviderGUID,
            {hns_syncable_prefs_ids::kSyncedDefaultPrivateSearchProviderGUID,
             syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
           {prefs::kSyncedDefaultPrivateSearchProviderData,
            {hns_syncable_prefs_ids::kSyncedDefaultPrivateSearchProviderData,
             syncer::PREFERENCES, /*is_history_opt_in_required*/ false}}});
  return kHnsCommonSyncablePrefsAllowlist;
}
}  // namespace
}  // namespace sync_preferences

#define GetSyncablePrefMetadata GetSyncablePrefMetadata_ChromiumOriginalImpl
#include "src/components/sync_preferences/common_syncable_prefs_database.cc"
#undef GetSyncablePrefMetadata

namespace sync_preferences {

absl::optional<SyncablePrefMetadata>
CommonSyncablePrefsDatabase::GetSyncablePrefMetadata(
    const std::string& pref_name) const {
  const auto* it = HnsSyncablePreferences().find(pref_name);
  if (it != HnsSyncablePreferences().end()) {
    return it->second;
  }
  return GetSyncablePrefMetadata_ChromiumOriginalImpl(pref_name);
}

absl::optional<SyncablePrefMetadata>
CommonSyncablePrefsDatabase::GetSyncablePrefMetadata_ChromiumImpl(
    const std::string& pref_name) const {
  return GetSyncablePrefMetadata(pref_name);
}

}  // namespace sync_preferences
