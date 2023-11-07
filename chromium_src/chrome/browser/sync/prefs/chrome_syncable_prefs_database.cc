/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/sync/prefs/chrome_syncable_prefs_database.h"

#include "base/containers/fixed_flat_map.h"
#include "base/strings/string_piece.h"

namespace browser_sync {
namespace {
namespace hns_syncable_prefs_ids {
enum {
  // Starts at 300000 to avoid clashing with the Chromium's syncable_prefs_ids:
  // chrome_syncable_prefs_database.cc starts at 100000
  // ios_chrome_syncable_prefs_database.cc starts at 200000
  kProfileContentSettingsExceptionsShieldsAds = 300000,
  kProfileContentSettingsExceptionsTrackers = 300001,
  kProfileContentSettingsExceptionsHttpsUpgrades = 300002,
  kProfileContentSettingsExceptionsHttpUpgradableResources = 300003,
  kProfileContentSettingsExceptionsReferrers = 300004,
  kProfileContentSettingsExceptionsShieldsCookiesV3 = 300005,
  kProfileContentSettingsExceptionsCosmeticFiltering = 300006,
  kProfileContentSettingsExceptionsFingerprintingV2 = 300007,
  kProfileContentSettingsExceptionsHnsShields = 300008,
  kProfileContentSettingsExceptionsHnsSpeedreader = 300009,
  kProfileDefaultContentSettingValuesShieldsAds = 300010,
  kProfileDefaultContentSettingValuesTrackers = 300011,
  kProfileDefaultContentSettingValuesHttpsUpgrades = 300012,
  kProfileDefaultContentSettingValuesHttpUpgradableResources = 300013,
  kProfileDefaultContentSettingValuesReferrers = 300014,
  kProfileDefaultContentSettingValuesShieldsCookiesV3 = 300015,
  kProfileDefaultContentSettingValuesCosmeticFiltering = 300016,
  kProfileDefaultContentSettingValuesFingerprintingV2 = 300017,
  kProfileDefaultContentSettingValuesHnsShields = 300018,
  kProfileDefaultContentSettingValuesHnsSpeedreader = 300019,
};
}  // namespace hns_syncable_prefs_ids

const auto& HnsSyncablePreferences() {
  static const auto kHnsSyncablePrefsAllowList = base::MakeFixedFlatMap<
      base::StringPiece, sync_preferences::SyncablePrefMetadata>({
      {"profile.content_settings.exceptions.shieldsAds",
       {hns_syncable_prefs_ids::kProfileContentSettingsExceptionsShieldsAds,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.trackers",
       {hns_syncable_prefs_ids::kProfileContentSettingsExceptionsTrackers,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.httpsUpgrades",
       {hns_syncable_prefs_ids::
            kProfileContentSettingsExceptionsHttpsUpgrades,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.httpUpgradableResources",
       {hns_syncable_prefs_ids::
            kProfileContentSettingsExceptionsHttpUpgradableResources,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.referrers",
       {hns_syncable_prefs_ids::kProfileContentSettingsExceptionsReferrers,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.shieldsCookiesV3",
       {hns_syncable_prefs_ids::
            kProfileContentSettingsExceptionsShieldsCookiesV3,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.cosmeticFiltering",
       {hns_syncable_prefs_ids::
            kProfileContentSettingsExceptionsCosmeticFiltering,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.fingerprintingV2",
       {hns_syncable_prefs_ids::
            kProfileContentSettingsExceptionsFingerprintingV2,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.hnsShields",
       {hns_syncable_prefs_ids::kProfileContentSettingsExceptionsHnsShields,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.content_settings.exceptions.hnsSpeedreader",
       {hns_syncable_prefs_ids::
            kProfileContentSettingsExceptionsHnsSpeedreader,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.shieldsAds",
       {hns_syncable_prefs_ids::kProfileDefaultContentSettingValuesShieldsAds,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.trackers",
       {hns_syncable_prefs_ids::kProfileDefaultContentSettingValuesTrackers,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.httpsUpgrades",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesHttpsUpgrades,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.httpUpgradableResources",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesHttpUpgradableResources,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.referrers",
       {hns_syncable_prefs_ids::kProfileDefaultContentSettingValuesReferrers,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.shieldsCookiesV3",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesShieldsCookiesV3,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.cosmeticFiltering",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesCosmeticFiltering,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.fingerprintingV2",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesFingerprintingV2,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.hnsShields",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesHnsShields,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
      {"profile.default_content_setting_values.hnsSpeedreader",
       {hns_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesHnsSpeedreader,
        syncer::PREFERENCES, /*is_history_opt_in_required*/ false}},
  });
  return kHnsSyncablePrefsAllowList;
}
}  // namespace
}  // namespace browser_sync

#define GetSyncablePrefMetadata GetSyncablePrefMetadata_ChromiumImpl
#include "src/chrome/browser/sync/prefs/chrome_syncable_prefs_database.cc"
#undef GetSyncablePrefMetadata

namespace browser_sync {

absl::optional<sync_preferences::SyncablePrefMetadata>
ChromeSyncablePrefsDatabase::GetSyncablePrefMetadata(
    const std::string& pref_name) const {
  const auto* it = HnsSyncablePreferences().find(pref_name);
  if (it != HnsSyncablePreferences().end()) {
    return it->second;
  }
  return GetSyncablePrefMetadata_ChromiumImpl(pref_name);
}

}  // namespace browser_sync
