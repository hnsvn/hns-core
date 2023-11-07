/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/sync/hns_sync_worker.h"

#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/json/json_writer.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/hns_sync/crypto/crypto.h"
#include "hns/components/hns_sync/qr_code_data.h"
#include "hns/components/hns_sync/qr_code_validator.h"
#include "hns/components/hns_sync/sync_service_impl_helper.h"
#include "hns/components/hns_sync/time_limited_words.h"
#include "hns/components/sync/service/hns_sync_service_impl.h"
#include "hns/components/sync_device_info/hns_device_info.h"
#include "components/sync/protocol/sync_protocol_error.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_impl.h"
#include "components/sync/service/sync_service_observer.h"
#include "components/sync_device_info/device_info.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"
#include "ios/chrome/browser/sync/sync_service_factory.h"
#include "ios/chrome/browser/sync/sync_setup_service.h"
#include "ios/chrome/browser/sync/sync_setup_service_factory.h"
#include "ios/web/public/thread/web_thread.h"

namespace {
static const size_t SEED_BYTES_COUNT = 32u;
}  // namespace

HnsSyncDeviceTracker::HnsSyncDeviceTracker(
    syncer::DeviceInfoTracker* device_info_tracker,
    const base::RepeatingCallback<void()>& on_device_info_changed_callback)
    : on_device_info_changed_callback_(on_device_info_changed_callback) {
  DCHECK(device_info_tracker);
  device_info_tracker_observer_.Observe(device_info_tracker);
}

HnsSyncDeviceTracker::~HnsSyncDeviceTracker() {
  // Observer will be removed by ScopedObservation
}

void HnsSyncDeviceTracker::OnDeviceInfoChange() {
  if (on_device_info_changed_callback_) {
    on_device_info_changed_callback_.Run();
  }
}

HnsSyncServiceTracker::HnsSyncServiceTracker(
    syncer::SyncServiceImpl* sync_service_impl,
    const base::RepeatingCallback<void()>& on_state_changed_callback,
    const base::RepeatingCallback<void()>& on_sync_shutdown_callback)
    : on_state_changed_callback_(on_state_changed_callback),
      on_sync_shutdown_callback_(on_sync_shutdown_callback) {
  DCHECK(sync_service_impl);
  sync_service_observer_.Observe(sync_service_impl);
}

HnsSyncServiceTracker::~HnsSyncServiceTracker() {
  // Observer will be removed by ScopedObservation
}

void HnsSyncServiceTracker::OnStateChanged(syncer::SyncService* sync) {
  if (on_state_changed_callback_) {
    on_state_changed_callback_.Run();
  }
}

void HnsSyncServiceTracker::OnSyncShutdown(syncer::SyncService* sync) {
  if (on_sync_shutdown_callback_) {
    on_sync_shutdown_callback_.Run();
  }
}

HnsSyncWorker::HnsSyncWorker(ChromeBrowserState* browser_state)
    : browser_state_(browser_state) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
}

HnsSyncWorker::~HnsSyncWorker() {
  // Observer will be removed by ScopedObservation
}

bool HnsSyncWorker::RequestSync() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  if (!sync_service_observer_.IsObservingSource(sync_service)) {
    sync_service_observer_.AddObservation(sync_service);
  }

  sync_service->SetSyncFeatureRequested();

  return true;
}

const syncer::DeviceInfo* HnsSyncWorker::GetLocalDeviceInfo() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!device_info_service) {
    return nullptr;
  }

  return device_info_service->GetLocalDeviceInfoProvider()
      ->GetLocalDeviceInfo();
}

std::vector<std::unique_ptr<syncer::HnsDeviceInfo>>
HnsSyncWorker::GetDeviceList() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!device_info_service) {
    return std::vector<std::unique_ptr<syncer::HnsDeviceInfo>>();
  }

  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  return tracker->GetAllHnsDeviceInfo();
}

std::string HnsSyncWorker::GetOrCreateSyncCode() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::HnsSyncServiceImpl* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service) {
    sync_code = sync_service->GetOrCreateSyncCode();
  }

  CHECK(hns_sync::crypto::IsPassphraseValid(sync_code));
  return sync_code;
}

bool HnsSyncWorker::IsValidSyncCode(const std::string& sync_code) {
  std::vector<uint8_t> seed;
  if (!hns_sync::crypto::PassphraseToBytes32(sync_code, &seed)) {
    return false;
  }
  return seed.size() == SEED_BYTES_COUNT;
}

bool HnsSyncWorker::SetSyncCode(const std::string& sync_code) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  if (sync_code.empty()) {
    return false;
  }

  syncer::HnsSyncServiceImpl* sync_service = GetSyncService();
  if (!sync_service || !sync_service->SetSyncCode(sync_code)) {
    const std::string error_msg = sync_service
                                      ? "invalid sync code:" + sync_code
                                      : "sync service is not available";
    LOG(ERROR) << error_msg;
    return false;
  }

  passphrase_ = sync_code;
  return true;
}

std::string HnsSyncWorker::GetSyncCodeFromHexSeed(
    const std::string& hex_code_seed) {
  DCHECK(!hex_code_seed.empty());

  std::vector<uint8_t> bytes;
  std::string sync_code_words;
  if (base::HexStringToBytes(hex_code_seed, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size() == SEED_BYTES_COUNT) {
      sync_code_words = hns_sync::crypto::PassphraseFromBytes32(bytes);
      if (sync_code_words.empty()) {
        VLOG(1) << __func__ << " PassphraseFromBytes32 failed for "
                << hex_code_seed;
      }
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }

    DCHECK_NE(sync_code_words, "");
  } else {
    VLOG(1) << __func__ << " HexStringToBytes failed for hex_code_seed";
  }
  return sync_code_words;
}

std::string HnsSyncWorker::GetHexSeedFromSyncCode(
    const std::string& code_words) {
  DCHECK(!code_words.empty());

  std::string sync_code_hex;
  std::vector<uint8_t> bytes;
  if (hns_sync::crypto::PassphraseToBytes32(code_words, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size() == SEED_BYTES_COUNT) {
      sync_code_hex = base::HexEncode(&bytes.at(0), bytes.size());
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }
  } else {
    VLOG(1) << __func__ << " PassphraseToBytes32 failed for " << code_words;
  }
  return sync_code_hex;
}

std::string HnsSyncWorker::GetQrCodeJsonFromHexSeed(
    const std::string& hex_seed) {
  DCHECK(!hex_seed.empty());
  return hns_sync::QrCodeData::CreateWithActualDate(hex_seed)->ToJson();
}

hns_sync::QrCodeDataValidationResult
HnsSyncWorker::GetQrCodeValidationResult(const std::string json) {
  DCHECK(!json.empty());
  return hns_sync::QrCodeDataValidator::ValidateQrDataJson(json);
}

hns_sync::TimeLimitedWords::ValidationStatus
HnsSyncWorker::GetWordsValidationResult(
    const std::string time_limited_words) {
  DCHECK(!time_limited_words.empty());
  auto words_with_status =
      hns_sync::TimeLimitedWords::Parse(time_limited_words);
  if (words_with_status.has_value()) {
    return hns_sync::TimeLimitedWords::ValidationStatus::kValid;
  } else {
    return words_with_status.error();
  }
}

std::string HnsSyncWorker::GetWordsFromTimeLimitedWords(
    const std::string& time_limited_words) {
  DCHECK(!time_limited_words.empty());
  auto words_with_status =
      hns_sync::TimeLimitedWords::Parse(time_limited_words);
  DCHECK(words_with_status.has_value());
  return words_with_status.value();
}

std::string HnsSyncWorker::GetTimeLimitedWordsFromWords(
    const std::string& words) {
  DCHECK(!words.empty());
  auto generate_result = hns_sync::TimeLimitedWords::GenerateForNow(words);
  if (generate_result.has_value()) {
    return generate_result.value();
  } else {
    DCHECK(false);
    return std::string();
  }
}

std::string HnsSyncWorker::GetHexSeedFromQrCodeJson(const std::string& json) {
  DCHECK(!json.empty());
  std::unique_ptr<hns_sync::QrCodeData> qr_data =
      hns_sync::QrCodeData::FromJson(json);
  if (qr_data) {
    DCHECK(!GetSyncCodeFromHexSeed(qr_data->sync_code_hex).empty());
    return qr_data->sync_code_hex;
  }

  DCHECK(!GetSyncCodeFromHexSeed(json).empty());
  return json;
}

bool HnsSyncWorker::IsInitialSyncFeatureSetupComplete() {
  syncer::SyncService* sync_service = GetSyncService();
  return sync_service &&
         sync_service->GetUserSettings()->IsInitialSyncFeatureSetupComplete();
}

bool HnsSyncWorker::SetSetupComplete() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  sync_service->SetSyncFeatureRequested();

  if (!sync_service->GetUserSettings()->IsInitialSyncFeatureSetupComplete()) {
    sync_service->GetUserSettings()->SetInitialSyncFeatureSetupComplete(
        syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
  }

  return true;
}

void HnsSyncWorker::ResetSync() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::HnsSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  DCHECK(device_info_service);

  hns_sync::ResetSync(sync_service, device_info_service,
                        base::BindOnce(&HnsSyncWorker::OnResetDone,
                                        weak_ptr_factory_.GetWeakPtr()));
}

void HnsSyncWorker::DeleteDevice(const std::string& device_guid) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::HnsSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  DCHECK(device_info_service);

  hns_sync::DeleteDevice(sync_service, device_info_service, device_guid);
}

void HnsSyncWorker::SetJoinSyncChainCallback(
    base::OnceCallback<void(bool)> callback) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::HnsSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  sync_service->SetJoinChainResultCallback(std::move(callback));
}

void HnsSyncWorker::PermanentlyDeleteAccount(
    base::OnceCallback<void(const syncer::SyncProtocolError&)> callback) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::HnsSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  sync_service->PermanentlyDeleteAccount(std::move(callback));
}

syncer::HnsSyncServiceImpl* HnsSyncWorker::GetSyncService() const {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  return static_cast<syncer::HnsSyncServiceImpl*>(
      SyncServiceFactory::GetForBrowserState(browser_state_));
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

    VLOG(3) << "[HnsSync] " << __func__ << " SYNC_CREATED_NEW_PASSPHRASE";
  }
}

void HnsSyncWorker::SetDecryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->SetDecryptionPassphrase(this->passphrase_)) {
    VLOG(3) << "[HnsSync] " << __func__
            << " SYNC_ENTERED_EXISTING_PASSPHRASE";
  }
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

void HnsSyncWorker::OnSyncShutdown(syncer::SyncService* service) {
  if (sync_service_observer_.IsObservingSource(service)) {
    sync_service_observer_.RemoveObservation(service);
  }
}

void HnsSyncWorker::OnResetDone() {
  syncer::SyncService* sync_service = GetSyncService();
  if (sync_service && sync_service_observer_.IsObservingSource(sync_service)) {
    sync_service_observer_.RemoveObservation(sync_service);
  }
}

bool HnsSyncWorker::CanSyncFeatureStart() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  return sync_service->IsSyncFeatureEnabled();
}

bool HnsSyncWorker::IsSyncFeatureActive() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  return sync_service->IsSyncFeatureActive();
}
