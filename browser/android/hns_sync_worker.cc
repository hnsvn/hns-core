/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/android/hns_sync_worker.h"

#include <string>
#include <vector>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/json/json_writer.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"

#include "hns/build/android/jni_headers/HnsSyncWorker_jni.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/hns_sync/crypto/crypto.h"
#include "hns/components/hns_sync/qr_code_data.h"
#include "hns/components/hns_sync/qr_code_validator.h"
#include "hns/components/hns_sync/sync_service_impl_helper.h"
#include "hns/components/hns_sync/time_limited_words.h"
#include "hns/components/sync/service/hns_sync_service_impl.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/sync/device_info_sync_service_factory.h"
#include "chrome/browser/sync/sync_service_factory.h"

#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_user_settings.h"
#include "components/unified_consent/unified_consent_metrics.h"

#include "content/public/browser/browser_thread.h"

#include "third_party/leveldatabase/src/include/leveldb/db.h"

// TODO(alexeybarabash): consider use of java SyncServiceImpl methods:
//    addSyncStateChangedListener
//    removeSyncStateChangedListener
//    requestStart
//    requestStop
//    setInitialSyncFeatureSetupComplete
//    isInitialSyncFeatureSetupComplete

using base::android::ConvertUTF8ToJavaString;
using content::BrowserThread;

namespace {
static const size_t SEED_BYTES_COUNT = 32u;
}  // namespace

namespace chrome {
namespace android {

// Keep this to clear V1 stuff on migrating
#define DB_FILE_NAME      "hns_sync_db"

HnsSyncWorker::HnsSyncWorker(JNIEnv* env,
                                 const base::android::JavaRef<jobject>& obj)
    : weak_java_hns_sync_worker_(env, obj) {
  Java_HnsSyncWorker_setNativePtr(env, obj, reinterpret_cast<intptr_t>(this));

  profile_ = ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
  DCHECK_NE(profile_, nullptr);
}

HnsSyncWorker::~HnsSyncWorker() {}

void HnsSyncWorker::Destroy(JNIEnv* env) {
  delete this;
}

static void JNI_HnsSyncWorker_DestroyV1LevelDb(JNIEnv* env) {
  base::FilePath app_data_path;
  base::PathService::Get(base::DIR_ANDROID_APP_DATA, &app_data_path);
  base::FilePath dbFilePath = app_data_path.Append(DB_FILE_NAME);

  leveldb::Status status =
      leveldb::DestroyDB(dbFilePath.value().c_str(), leveldb::Options());
  VLOG(3) << "[HnsSync] " << __func__ << " destroy DB status is "
          << status.ToString();
}

static void JNI_HnsSyncWorker_MarkSyncV1WasEnabledAndMigrated(JNIEnv* env) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  Profile* profile =
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
  hns_sync::Prefs hns_sync_prefs(profile->GetPrefs());
  hns_sync_prefs.SetSyncV1WasEnabled();
  hns_sync_prefs.SetSyncV1Migrated(true);
  VLOG(3) << "[HnsSync] " << __func__ << " done";
}

base::android::ScopedJavaLocalRef<jstring> HnsSyncWorker::GetSyncCodeWords(
    JNIEnv* env) {
  auto* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service)
    sync_code = sync_service->GetOrCreateSyncCode();

  CHECK(hns_sync::crypto::IsPassphraseValid(sync_code));

  return ConvertUTF8ToJavaString(env, sync_code);
}

void HnsSyncWorker::SaveCodeWords(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& passphrase) {
  std::string str_passphrase =
      base::android::ConvertJavaStringToUTF8(passphrase);

  auto* sync_service = GetSyncService();
  if (!sync_service || !sync_service->SetSyncCode(str_passphrase)) {
    const std::string error_msg =
      sync_service
      ? "invalid sync code:" + str_passphrase
      : "sync service is not available";
    LOG(ERROR) << error_msg;
    return;
  }

  passphrase_ = str_passphrase;
}

syncer::HnsSyncServiceImpl* HnsSyncWorker::GetSyncService() const {
  return SyncServiceFactory::IsSyncAllowed(profile_)
             ? static_cast<syncer::HnsSyncServiceImpl*>(
                   SyncServiceFactory::GetForProfile(profile_))
             : nullptr;
}

// Most of methods below were taken from by PeopleHandler class to
// bring the logic of enabling / disabling sync from deskop to Android

void HnsSyncWorker::RequestSync(JNIEnv* env) {
  syncer::SyncService* service = SyncServiceFactory::GetForProfile(profile_);

  if (service && !sync_service_observer_.IsObservingSource(service)) {
    sync_service_observer_.AddObservation(service);
  }

  // Mark Sync as requested by the user. It might already be requested, but
  // it's not if this is either the first time the user is setting up Sync, or
  // Sync was set up but then was reset via the dashboard. This also pokes the
  // SyncService to start up immediately, i.e. bypass deferred startup.
  if (service) {
    service->SetSyncFeatureRequested();
  }
}

void HnsSyncWorker::MarkFirstSetupComplete() {
  syncer::SyncService* service = GetSyncService();

  // The sync service may be nullptr if it has been just disabled by policy.
  if (!service)
    return;

  service->SetSyncFeatureRequested();

  // If the first-time setup is already complete, there's nothing else to do.
  if (service->GetUserSettings()->IsInitialSyncFeatureSetupComplete()) {
    return;
  }

  unified_consent::metrics::RecordSyncSetupDataTypesHistrogam(
      service->GetUserSettings());

  // We're done configuring, so notify SyncService that it is OK to start
  // syncing.
  service->GetUserSettings()->SetInitialSyncFeatureSetupComplete(
      syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
}

void HnsSyncWorker::FinalizeSyncSetup(JNIEnv* env) {
  MarkFirstSetupComplete();
}

bool HnsSyncWorker::IsInitialSyncFeatureSetupComplete(JNIEnv* env) {
  syncer::SyncService* sync_service = GetSyncService();
  return sync_service &&
         sync_service->GetUserSettings()->IsInitialSyncFeatureSetupComplete();
}

void HnsSyncWorker::ResetSync(JNIEnv* env) {
  auto* sync_service = GetSyncService();

  if (!sync_service)
    return;

  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  hns_sync::ResetSync(sync_service, device_info_sync_service,
                        base::BindOnce(&HnsSyncWorker::OnResetDone,
                                       weak_ptr_factory_.GetWeakPtr()));
}

bool HnsSyncWorker::GetSyncV1WasEnabled(JNIEnv* env) {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  bool sync_v1_was_enabled = hns_sync_prefs.IsSyncV1Enabled();
  return sync_v1_was_enabled;
}

bool HnsSyncWorker::GetSyncV2MigrateNoticeDismissed(JNIEnv* env) {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  bool sync_v2_migration_notice_dismissed =
      hns_sync_prefs.IsSyncMigrateNoticeDismissed();
  return sync_v2_migration_notice_dismissed;
}

void HnsSyncWorker::SetSyncV2MigrateNoticeDismissed(
    JNIEnv* env,
    bool sync_v2_migration_notice_dismissed) {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  hns_sync_prefs.SetDismissSyncMigrateNotice(
      sync_v2_migration_notice_dismissed);
}

void HnsSyncWorker::OnResetDone() {
  syncer::SyncService* sync_service = GetSyncService();
  if (sync_service) {
    if (sync_service_observer_.IsObservingSource(sync_service)) {
      sync_service_observer_.RemoveObservation(sync_service);
    }
  }
}

void HnsSyncWorker::SetEncryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(!sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->IsCustomPassphraseAllowed() &&
      !sync_user_settings->IsUsingExplicitPassphrase() &&
      !sync_user_settings->IsTrustedVaultKeyRequired()) {
    sync_user_settings->SetEncryptionPassphrase(this->passphrase_);
  }
}

void HnsSyncWorker::SetDecryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());
  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(sync_user_settings->IsPassphraseRequired());

  bool set_decryption_result =
      sync_user_settings->SetDecryptionPassphrase(this->passphrase_);
  DCHECK(set_decryption_result);
  VLOG_IF(3, !set_decryption_result)
      << "[HnsSync] " << __func__ << " SetDecryptionPassphrase failed";
}

void HnsSyncWorker::OnStateChanged(syncer::SyncService* service) {
  // If the sync engine has shutdown for some reason, just give up
  if (!service || !service->IsEngineInitialized()) {
    VLOG(3) << "[HnsSync] " << __func__ << " sync engine is not initialized";
    return;
  }

  if (this->passphrase_.empty()) {
    VLOG(3) << "[HnsSync] " << __func__ << " empty passphrase";
    return;
  }

  if (service->GetUserSettings()->IsPassphraseRequired()) {
    SetDecryptionPassphrase(service);
  } else {
    SetEncryptionPassphrase(service);
  }
}

namespace {
std::string GetErrorDescription(
    const syncer::SyncProtocolError& sync_protocol_error) {
  if (sync_protocol_error.error_type == syncer::SYNC_SUCCESS) {
    return std::string();
  } else if (sync_protocol_error.error_type != syncer::SYNC_SUCCESS &&
             sync_protocol_error.error_description.empty()) {
    return GetSyncErrorTypeString(sync_protocol_error.error_type);
  } else {
    return sync_protocol_error.error_description;
  }
}

void NativePermanentlyDeleteAccountCallback(
    JNIEnv* env,
    const base::android::ScopedJavaGlobalRef<jobject>& callback,
    const syncer::SyncProtocolError& sync_protocol_error) {
  std::string error_description = GetErrorDescription(sync_protocol_error);

  Java_HnsSyncWorker_onPermanentlyDeleteAccountResult(
      env, callback, ConvertUTF8ToJavaString(env, error_description));
}
}  // namespace

void HnsSyncWorker::PermanentlyDeleteAccount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  auto* sync_service = GetSyncService();
  CHECK_NE(sync_service, nullptr);

  base::android::ScopedJavaGlobalRef<jobject> java_callback;
  java_callback.Reset(env, callback);

  sync_service->PermanentlyDeleteAccount(base::BindOnce(
      &NativePermanentlyDeleteAccountCallback, env, java_callback));
}

namespace {

base::android::ScopedJavaLocalRef<jobject> GetJavaBoolean(
    JNIEnv* env,
    const bool& native_bool) {
  jclass booleanClass = env->FindClass("java/lang/Boolean");
  jmethodID methodID = env->GetMethodID(booleanClass, "<init>", "(Z)V");
  jobject booleanObject = env->NewObject(booleanClass, methodID, native_bool);

  return base::android::ScopedJavaLocalRef<jobject>(env, booleanObject);
}

void NativeJoinSyncChainCallback(
    JNIEnv* env,
    const base::android::ScopedJavaGlobalRef<jobject>& callback,
    bool result) {
  Java_HnsSyncWorker_onJoinSyncChainResult(env, callback,
                                             GetJavaBoolean(env, result));
}

}  // namespace

void HnsSyncWorker::SetJoinSyncChainCallback(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  auto* sync_service = GetSyncService();
  CHECK_NE(sync_service, nullptr);

  base::android::ScopedJavaGlobalRef<jobject> java_callback;
  java_callback.Reset(env, callback);

  sync_service->SetJoinChainResultCallback(
      base::BindOnce(&NativeJoinSyncChainCallback, env, java_callback));
}

void HnsSyncWorker::ClearAccountDeletedNoticePending(JNIEnv* env) {
  Profile* profile =
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
  hns_sync::Prefs hns_sync_prefs(profile->GetPrefs());
  hns_sync_prefs.SetSyncAccountDeletedNoticePending(false);
}

bool HnsSyncWorker::IsAccountDeletedNoticePending(JNIEnv* env) {
  Profile* profile =
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
  hns_sync::Prefs hns_sync_prefs(profile->GetPrefs());
  return hns_sync_prefs.IsSyncAccountDeletedNoticePending();
}

static void JNI_HnsSyncWorker_Init(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  new HnsSyncWorker(env, jcaller);
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsSyncWorker_GetSeedHexFromWords(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& seed_words) {
  std::string str_seed_words =
      base::android::ConvertJavaStringToUTF8(seed_words);
  DCHECK(!str_seed_words.empty());

  std::string sync_code_hex;
  std::vector<uint8_t> bytes;
  if (hns_sync::crypto::PassphraseToBytes32(str_seed_words, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    sync_code_hex = base::HexEncode(&bytes.at(0), bytes.size());
  } else {
    VLOG(1) << __func__ << " PassphraseToBytes32 failed for " << str_seed_words;
  }

  return ConvertUTF8ToJavaString(env, sync_code_hex);
}

std::string GetWordsFromSeedHex(const std::string& str_seed_hex) {
  DCHECK(!str_seed_hex.empty());

  std::vector<uint8_t> bytes;
  std::string sync_code_words;
  if (base::HexStringToBytes(str_seed_hex, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size(), SEED_BYTES_COUNT) {
      sync_code_words = hns_sync::crypto::PassphraseFromBytes32(bytes);
      if (sync_code_words.empty()) {
        VLOG(1) << __func__ << " PassphraseFromBytes32 failed for "
                << str_seed_hex;
      }
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }
    DCHECK_NE(sync_code_words, "");
  } else {
    VLOG(1) << __func__ << " HexStringToBytes failed for " << str_seed_hex;
  }

  return sync_code_words;
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsSyncWorker_GetWordsFromSeedHex(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& seed_hex) {
  std::string str_seed_hex = base::android::ConvertJavaStringToUTF8(seed_hex);
  std::string sync_code_words = GetWordsFromSeedHex(str_seed_hex);
  return ConvertUTF8ToJavaString(env, sync_code_words);
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsSyncWorker_GetQrDataJson(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& seed_hex) {
  std::string str_seed_hex = base::android::ConvertJavaStringToUTF8(seed_hex);
  DCHECK(!str_seed_hex.empty());

  const std::string qr_code_string =
      hns_sync::QrCodeData::CreateWithActualDate(str_seed_hex)->ToJson();

  return ConvertUTF8ToJavaString(env, qr_code_string);
}

int JNI_HnsSyncWorker_GetQrCodeValidationResult(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& json_qr) {
  std::string str_json_qr = base::android::ConvertJavaStringToUTF8(json_qr);
  DCHECK(!str_json_qr.empty());
  return static_cast<int>(
      hns_sync::QrCodeDataValidator::ValidateQrDataJson(str_json_qr));
}

int JNI_HnsSyncWorker_GetWordsValidationResult(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& time_limited_words) {
  std::string str_time_limited_words =
      base::android::ConvertJavaStringToUTF8(time_limited_words);
  DCHECK(!str_time_limited_words.empty());

  auto pure_words_with_status =
      hns_sync::TimeLimitedWords::Parse(str_time_limited_words);

  if (pure_words_with_status.has_value()) {
    using ValidationStatus = hns_sync::TimeLimitedWords::ValidationStatus;
    return static_cast<int>(ValidationStatus::kValid);
  }
  return static_cast<int>(pure_words_with_status.error());
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsSyncWorker_GetPureWordsFromTimeLimited(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& time_limited_words) {
  std::string str_time_limited_words =
      base::android::ConvertJavaStringToUTF8(time_limited_words);
  DCHECK(!str_time_limited_words.empty());

  auto pure_words_with_status =
      hns_sync::TimeLimitedWords::Parse(str_time_limited_words);
  DCHECK(pure_words_with_status.has_value());

  return base::android::ConvertUTF8ToJavaString(env,
                                                pure_words_with_status.value());
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsSyncWorker_GetTimeLimitedWordsFromPure(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& pure_words) {
  std::string str_pure_words =
      base::android::ConvertJavaStringToUTF8(pure_words);
  DCHECK(!str_pure_words.empty());

  auto time_limited_words =
      hns_sync::TimeLimitedWords::GenerateForNow(str_pure_words);

  DCHECK(time_limited_words.has_value());
  return base::android::ConvertUTF8ToJavaString(env,
                                                time_limited_words.value());
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsSyncWorker_GetSeedHexFromQrJson(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& json_qr) {
  std::string str_json_qr = base::android::ConvertJavaStringToUTF8(json_qr);
  DCHECK(!str_json_qr.empty());

  auto qr_data = hns_sync::QrCodeData::FromJson(str_json_qr);

  std::string result;
  if (qr_data) {
    result = qr_data->sync_code_hex;
  } else {
    result = str_json_qr;
  }

  DCHECK(!GetWordsFromSeedHex(result).empty());

  return ConvertUTF8ToJavaString(env, result);
}

}  // namespace android
}  // namespace chrome
