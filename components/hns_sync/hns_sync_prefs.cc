/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_sync/hns_sync_prefs.h"

#include <utility>

#include "base/base64.h"
#include "base/logging.h"
#include "components/os_crypt/sync/os_crypt.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/scoped_user_pref_update.h"

namespace hns_sync {
namespace {

// Stored as bip39 keywords (encrypted)
const char kSyncV2Seed[] = "hns_sync_v2.seed";
// Indicate whether migration has been done from v1 to v2
const char kSyncV1Migrated[] = "hns_sync_v2.v1_migrated";
// Indicate all meta info set in V1 has been stripped in
// HnsBookmarkModelLoadedObserver
const char kSyncV1MetaInfoCleared[] = "hns_sync_v2.v1_meta_info_cleared";
// Has dismissed message about migration to sync v2
const char kSyncV2MigrateNoticeDismissed[] =
    "hns_sync_v2.migrate_notice_dismissed";
const char kSyncFailedDecryptSeedNoticeDismissed[] =
    "hns_sync_v2.failed_decrypt_seed_notice_dismissed";
const char kSyncAccountDeletedNoticePending[] =
    "hns_sync_v2.account_deleted_notice_pending";

// Deprecated
// ============================================================================
const char kSyncSeed[] = "hns_sync.seed";
const char kSyncEnabled[] = "hns_sync.enabled";
const char kSyncDeviceId[] = "hns_sync.device_id";
const char kSyncDeviceIdV2[] = "hns_sync.device_id_v2";
const char kSyncDeviceObjectId[] = "hns_sync.device_object_id";
const char kSyncPrevSeed[] = "hns_sync.previous_seed";
const char kSyncDeviceName[] = "hns_sync.device_name";
const char kSyncBookmarksBaseOrder[] = "hns_sync.bookmarks_base_order";
const char kSyncBookmarksEnabled[] = "hns_sync.bookmarks_enabled";
const char kSyncSiteSettingsEnabled[] = "hns_sync.site_settings_enabled";
const char kSyncHistoryEnabled[] = "hns_sync.history_enabled";
const char kSyncLatestRecordTime[] = "hns_sync.latest_record_time";
const char kSyncLatestDeviceRecordTime[] =
    "hns_sync.latest_device_record_time";
const char kSyncLastFetchTime[] = "hns_sync.last_fetch_time";
const char kSyncLastCompactTimeBookmarks[] =
    "hns_sync.last_compact_time.bookmarks";
const char kSyncDeviceList[] = "hns_sync.device_list";
const char kSyncApiVersion[] = "hns_sync.api_version";
const char kSyncMigrateBookmarksVersion[]
                                       = "hns_sync.migrate_bookmarks_version";
const char kSyncRecordsToResend[] = "hns_sync_records_to_resend";
const char kSyncRecordsToResendMeta[] = "hns_sync_records_to_resend_meta";
const char kDuplicatedBookmarksRecovered[] =
    "hns_sync_duplicated_bookmarks_recovered";
const char kDuplicatedBookmarksMigrateVersion[] =
    "hns_sync_duplicated_bookmarks_migrate_version";
// ============================================================================
}  // namespace

Prefs::Prefs(PrefService* pref_service) : pref_service_(*pref_service) {}

Prefs::~Prefs() = default;

// static
void Prefs::RegisterProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterStringPref(kSyncV2Seed, std::string());
  registry->RegisterBooleanPref(kSyncV1Migrated, false);
  registry->RegisterBooleanPref(kSyncV1MetaInfoCleared, false);
  registry->RegisterBooleanPref(kSyncV2MigrateNoticeDismissed, false);
  registry->RegisterBooleanPref(kSyncFailedDecryptSeedNoticeDismissed, false);
  registry->RegisterBooleanPref(kSyncAccountDeletedNoticePending, false);

  // Deprecated
  // ============================================================================
  registry->RegisterStringPref(kSyncSeed, std::string());
  registry->RegisterBooleanPref(kSyncEnabled, false);
  registry->RegisterStringPref(kSyncDeviceId, std::string());
  registry->RegisterStringPref(kSyncDeviceIdV2, std::string());
  registry->RegisterStringPref(kSyncDeviceObjectId, std::string());
  registry->RegisterStringPref(kSyncPrevSeed, std::string());
  registry->RegisterStringPref(kSyncDeviceName, std::string());
  registry->RegisterStringPref(kSyncBookmarksBaseOrder, std::string());
  registry->RegisterBooleanPref(kSyncBookmarksEnabled, false);
  registry->RegisterBooleanPref(kSyncSiteSettingsEnabled, false);
  registry->RegisterBooleanPref(kSyncHistoryEnabled, false);
  registry->RegisterTimePref(kSyncLatestRecordTime, base::Time());
  registry->RegisterTimePref(kSyncLatestDeviceRecordTime, base::Time());
  registry->RegisterTimePref(kSyncLastFetchTime, base::Time());
  registry->RegisterTimePref(kSyncLastCompactTimeBookmarks,
                             base::Time());
  registry->RegisterStringPref(kSyncDeviceList, std::string());
  registry->RegisterStringPref(kSyncApiVersion, std::string("0"));
  registry->RegisterIntegerPref(kSyncMigrateBookmarksVersion, 0);
  registry->RegisterListPref(kSyncRecordsToResend);
  registry->RegisterDictionaryPref(kSyncRecordsToResendMeta);
  registry->RegisterBooleanPref(kDuplicatedBookmarksRecovered, false);
  registry->RegisterIntegerPref(kDuplicatedBookmarksMigrateVersion, 0);
}

// static
std::string Prefs::GetSeedPath() {
  return kSyncV2Seed;
}

std::string Prefs::GetSeed(bool* failed_to_decrypt) const {
  CHECK(failed_to_decrypt);
  *failed_to_decrypt = true;

  const std::string encoded_seed = pref_service_->GetString(kSyncV2Seed);
  if (encoded_seed.empty()) {
    *failed_to_decrypt = false;
    return std::string();
  }

  std::string encrypted_seed;
  if (!base::Base64Decode(encoded_seed, &encrypted_seed)) {
    LOG(ERROR) << "base64 decode sync seed failure";
    return std::string();
  }

  std::string seed;
  if (!OSCrypt::DecryptString(encrypted_seed, &seed)) {
    LOG(ERROR) << "Decrypt sync seed failure";
    return std::string();
  }

  *failed_to_decrypt = false;
  return seed;
}

bool Prefs::SetSeed(const std::string& seed) {
  DCHECK(!seed.empty());
  std::string encrypted_seed;
  if (!OSCrypt::EncryptString(seed, &encrypted_seed)) {
    LOG(ERROR) << "Encrypt sync seed failure";
    return false;
  }
  // String stored in prefs has to be UTF8 string so we use base64 to encode it.
  std::string encoded_seed;
  base::Base64Encode(encrypted_seed, &encoded_seed);
  pref_service_->SetString(kSyncV2Seed, encoded_seed);
  SetSyncAccountDeletedNoticePending(false);
  return true;
}

bool Prefs::IsSyncV1Migrated() const {
  return pref_service_->GetBoolean(kSyncV1Migrated);
}

void Prefs::SetSyncV1Migrated(bool is_migrated) {
  pref_service_->SetBoolean(kSyncV1Migrated, is_migrated);
}

bool Prefs::IsSyncV1MetaInfoCleared() const {
  return pref_service_->GetBoolean(kSyncV1MetaInfoCleared);
}

void Prefs::SetSyncV1MetaInfoCleared(bool is_cleared) {
  pref_service_->SetBoolean(kSyncV1MetaInfoCleared, is_cleared);
}

bool Prefs::IsSyncV1Enabled() const {
  return pref_service_->GetBoolean(kSyncEnabled);
}

#if BUILDFLAG(IS_ANDROID)
void Prefs::SetSyncV1WasEnabled() const {
  pref_service_->SetBoolean(kSyncEnabled, true);
}
#endif

bool Prefs::IsSyncMigrateNoticeDismissed() const {
  return pref_service_->GetBoolean(kSyncV2MigrateNoticeDismissed);
}

void Prefs::SetDismissSyncMigrateNotice(bool is_dismissed) {
  pref_service_->SetBoolean(kSyncV2MigrateNoticeDismissed, is_dismissed);
}

bool Prefs::IsFailedDecryptSeedNoticeDismissed() const {
  return pref_service_->GetBoolean(kSyncFailedDecryptSeedNoticeDismissed);
}

void Prefs::DismissFailedDecryptSeedNotice() {
  pref_service_->SetBoolean(kSyncFailedDecryptSeedNoticeDismissed, true);
}

bool Prefs::IsSyncAccountDeletedNoticePending() const {
  return pref_service_->GetBoolean(kSyncAccountDeletedNoticePending);
}

void Prefs::SetSyncAccountDeletedNoticePending(bool is_pending) {
  pref_service_->SetBoolean(kSyncAccountDeletedNoticePending, is_pending);
}

void Prefs::Clear() {
  pref_service_->ClearPref(kSyncV2Seed);
  pref_service_->ClearPref(kSyncFailedDecryptSeedNoticeDismissed);
}

void MigrateHnsSyncPrefs(PrefService* prefs) {
  // Added 11/2019
  prefs->ClearPref(kSyncPrevSeed);

  // Added 05/2020
  prefs->ClearPref(kSyncSeed);
  // Clear this prefs after almost every users have migrated to sync v2
  // prefs->ClearPref(kSyncEnabled);
  prefs->ClearPref(kDuplicatedBookmarksRecovered);
  prefs->ClearPref(kSyncDeviceId);
  prefs->ClearPref(kSyncDeviceIdV2);
  prefs->ClearPref(kSyncDeviceObjectId);
  prefs->ClearPref(kSyncDeviceName);
  prefs->ClearPref(kSyncBookmarksEnabled);
  prefs->ClearPref(kSyncBookmarksBaseOrder);
  prefs->ClearPref(kSyncSiteSettingsEnabled);
  prefs->ClearPref(kSyncHistoryEnabled);
  prefs->ClearPref(kSyncLatestRecordTime);
  prefs->ClearPref(kSyncLatestDeviceRecordTime);
  prefs->ClearPref(kSyncLastFetchTime);
  prefs->ClearPref(kSyncDeviceList);
  prefs->ClearPref(kSyncApiVersion);
  prefs->ClearPref(kSyncMigrateBookmarksVersion);
  prefs->ClearPref(kSyncRecordsToResend);
  prefs->ClearPref(kSyncRecordsToResendMeta);
  prefs->ClearPref(kDuplicatedBookmarksMigrateVersion);
}

}  // namespace hns_sync
