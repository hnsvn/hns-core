/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "hns/ios/browser/api/sync/hns_sync_api.h"

#import <CoreImage/CoreImage.h>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/functional/bind.h"
#include "base/json/json_writer.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/hns_sync/crypto/crypto.h"
#include "hns/components/hns_sync/qr_code_validator.h"
#include "hns/components/hns_sync/time_limited_words.h"
#include "hns/components/sync_device_info/hns_device_info.h"
#include "hns/ios/browser/api/sync/hns_sync_internals+private.h"
#include "hns/ios/browser/api/sync/hns_sync_worker.h"

#include "components/sync/protocol/sync_protocol_error.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_impl.h"
#include "components/sync/service/sync_service_observer.h"
#include "components/sync_device_info/device_info.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state_manager.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"
#include "ios/chrome/browser/sync/sync_service_factory.h"
#include "ios/chrome/browser/sync/sync_setup_service.h"
#include "ios/chrome/browser/sync/sync_setup_service_factory.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// MARK: - HnsSyncAPISyncProtocolErrorResult

HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultSuccess =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::SYNC_SUCCESS);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultNotMyBirthday =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::NOT_MY_BIRTHDAY);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultThrottled =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::THROTTLED);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultClearPending =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::CLEAR_PENDING);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultTransientError =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::TRANSIENT_ERROR);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultMigrationDone =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::MIGRATION_DONE);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultDisabledByAdmin = static_cast<NSInteger>(
        syncer::SyncProtocolErrorType::DISABLED_BY_ADMIN);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultPartialFailure =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::PARTIAL_FAILURE);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultDataObsolete = static_cast<NSInteger>(
        syncer::SyncProtocolErrorType::CLIENT_DATA_OBSOLETE);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultEncryptionObsolete =
        static_cast<NSInteger>(
            syncer::SyncProtocolErrorType::ENCRYPTION_OBSOLETE);
HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultUnknown =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::UNKNOWN_ERROR);

// MARK: - QrCodeDataValidationResult

HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultValid =
        static_cast<NSInteger>(hns_sync::QrCodeDataValidationResult::kValid);
HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultNotWellFormed =
        static_cast<NSInteger>(
            hns_sync::QrCodeDataValidationResult::kNotWellFormed);
HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultVersionDeprecated =
        static_cast<NSInteger>(
            hns_sync::QrCodeDataValidationResult::kVersionDeprecated);
HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultExpired = static_cast<NSInteger>(
        hns_sync::QrCodeDataValidationResult::kExpired);
HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultValidForTooLong =
        static_cast<NSInteger>(
            hns_sync::QrCodeDataValidationResult::kValidForTooLong);

// MARK: - TimeLimitedWords::ValidationStatus

HnsSyncAPIWordsValidationStatus const HnsSyncAPIWordsValidationStatusValid =
    static_cast<NSInteger>(
        hns_sync::TimeLimitedWords::ValidationStatus::kValid);
HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusNotValidPureWords = static_cast<NSInteger>(
        hns_sync::TimeLimitedWords::ValidationStatus::kNotValidPureWords);
HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusVersionDeprecated = static_cast<NSInteger>(
        hns_sync::TimeLimitedWords::ValidationStatus::kVersionDeprecated);
HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusExpired = static_cast<NSInteger>(
        hns_sync::TimeLimitedWords::ValidationStatus::kExpired);
HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusValidForTooLong = static_cast<NSInteger>(
        hns_sync::TimeLimitedWords::ValidationStatus::kValidForTooLong);
HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusWrongWordsNumber = static_cast<NSInteger>(
        hns_sync::TimeLimitedWords::ValidationStatus::kWrongWordsNumber);

// MARK: - HnsSyncDeviceObserver

@interface HnsSyncDeviceObserver : NSObject {
  std::unique_ptr<HnsSyncDeviceTracker> _device_observer;
}
@end

@implementation HnsSyncDeviceObserver

- (instancetype)initWithDeviceInfoTracker:(syncer::DeviceInfoTracker*)tracker
                                 callback:(void (^)())onDeviceInfoChanged {
  if ((self = [super init])) {
    _device_observer = std::make_unique<HnsSyncDeviceTracker>(
        tracker, base::BindRepeating(onDeviceInfoChanged));
  }
  return self;
}
@end

// MARK: - HnsSyncServiceObserver

@interface HnsSyncServiceObserver : NSObject {
  std::unique_ptr<HnsSyncServiceTracker> _service_tracker;
}
@end

@implementation HnsSyncServiceObserver

- (instancetype)initWithSyncServiceImpl:
                    (syncer::SyncServiceImpl*)syncServiceImpl
                   stateChangedCallback:(void (^)())onSyncServiceStateChanged
                   syncShutdownCallback:(void (^)())onSyncServiceShutdown {
  if ((self = [super init])) {
    _service_tracker = std::make_unique<HnsSyncServiceTracker>(
        syncServiceImpl, base::BindRepeating(onSyncServiceStateChanged),
        base::BindRepeating(onSyncServiceShutdown));
  }
  return self;
}
@end

// MARK: - HnsSyncAPI

@interface HnsSyncAPI () {
  std::unique_ptr<HnsSyncWorker> _worker;
  ChromeBrowserState* _chromeBrowserState;
}
@end

@implementation HnsSyncAPI

- (instancetype)initWithBrowserState:(ChromeBrowserState*)mainBrowserState {
  if ((self = [super init])) {
    _chromeBrowserState = mainBrowserState;
    _worker.reset(new HnsSyncWorker(_chromeBrowserState));
  }
  return self;
}

- (void)dealloc {
  _worker.reset();
  _chromeBrowserState = NULL;
}

- (bool)canSyncFeatureStart {
  return _worker->CanSyncFeatureStart();
}

- (bool)isSyncFeatureActive {
  return _worker->IsSyncFeatureActive();
}

- (bool)isInitialSyncFeatureSetupComplete {
  return _worker->IsInitialSyncFeatureSetupComplete();
}

- (void)setSetupComplete {
  _worker->SetSetupComplete();
}

- (void)requestSync {
  _worker->RequestSync();
}

- (bool)isValidSyncCode:(NSString*)syncCode {
  return _worker->IsValidSyncCode(base::SysNSStringToUTF8(syncCode));
}

- (NSString*)getSyncCode {
  std::string syncCode = _worker->GetOrCreateSyncCode();
  if (syncCode.empty()) {
    return nil;
  }

  return base::SysUTF8ToNSString(syncCode);
}

- (bool)setSyncCode:(NSString*)syncCode {
  return _worker->SetSyncCode(base::SysNSStringToUTF8(syncCode));
}

- (NSString*)syncCodeFromHexSeed:(NSString*)hexSeed {
  return base::SysUTF8ToNSString(
      _worker->GetSyncCodeFromHexSeed(base::SysNSStringToUTF8(hexSeed)));
}

- (NSString*)hexSeedFromSyncCode:(NSString*)syncCode {
  return base::SysUTF8ToNSString(
      _worker->GetHexSeedFromSyncCode(base::SysNSStringToUTF8(syncCode)));
}

- (NSString*)qrCodeJsonFromHexSeed:(NSString*)hexSeed {
  return base::SysUTF8ToNSString(
      _worker->GetQrCodeJsonFromHexSeed(base::SysNSStringToUTF8(hexSeed)));
}

- (HnsSyncAPIQrCodeDataValidationResult)getQRCodeValidationResult:
    (NSString*)json {
  return static_cast<HnsSyncAPIQrCodeDataValidationResult>(
      _worker->GetQrCodeValidationResult(base::SysNSStringToUTF8(json)));
}

- (HnsSyncAPIWordsValidationStatus)getWordsValidationResult:
    (NSString*)timeLimitedWords {
  return static_cast<HnsSyncAPIWordsValidationStatus>(
      _worker->GetWordsValidationResult(
          base::SysNSStringToUTF8(timeLimitedWords)));
}

- (NSString*)getWordsFromTimeLimitedWords:(NSString*)timeLimitedWords {
  return base::SysUTF8ToNSString(_worker->GetWordsFromTimeLimitedWords(
      base::SysNSStringToUTF8(timeLimitedWords)));
}

- (NSString*)getTimeLimitedWordsFromWords:(NSString*)words {
  return base::SysUTF8ToNSString(
      _worker->GetTimeLimitedWordsFromWords(base::SysNSStringToUTF8(words)));
}

- (NSString*)getHexSeedFromQrCodeJson:(NSString*)json {
  return base::SysUTF8ToNSString(
      _worker->GetHexSeedFromQrCodeJson(base::SysNSStringToUTF8(json)));
}

- (UIImage*)getQRCodeImage:(CGSize)size {
  std::vector<uint8_t> seed;
  std::string sync_code = _worker->GetOrCreateSyncCode();
  if (!hns_sync::crypto::PassphraseToBytes32(sync_code, &seed)) {
    return nil;
  }

  // QR code version 3 can only carry 84 bytes so we hex encode 32 bytes
  // seed then we will have 64 bytes input data
  const std::string sync_code_hex = base::HexEncode(seed.data(), seed.size());

  NSData* sync_code_data = [base::SysUTF8ToNSString(sync_code_hex.c_str())
      dataUsingEncoding:NSUTF8StringEncoding];  // NSISOLatin1StringEncoding

  if (!sync_code_data) {
    return nil;
  }

  CIFilter* filter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
  [filter setValue:sync_code_data forKey:@"inputMessage"];
  [filter setValue:@"H" forKey:@"inputCorrectionLevel"];

  CIImage* ciImage = [filter outputImage];
  if (ciImage) {
    CGFloat scaleX = size.width / ciImage.extent.size.width;
    CGFloat scaleY = size.height / ciImage.extent.size.height;
    CGAffineTransform transform = CGAffineTransformMakeScale(scaleX, scaleY);
    ciImage = [ciImage imageByApplyingTransform:transform];

    return [UIImage imageWithCIImage:ciImage
                               scale:[[UIScreen mainScreen] scale]
                         orientation:UIImageOrientationUp];
  }
  return nil;
}

- (NSString*)getDeviceListJSON {
  auto device_list = _worker->GetDeviceList();
  auto* local_device_info = _worker->GetLocalDeviceInfo();

  base::Value::List device_list_value;

  for (const auto& device : device_list) {
    auto device_value = device->ToValue();
    bool is_current_device =
        local_device_info ? local_device_info->guid() == device->guid() : false;
    device_value.Set("isCurrentDevice", is_current_device);
    device_value.Set("guid", device->guid());
    device_value.Set("supportsSelfDelete", device->is_self_delete_supported());
    device_list_value.Append(base::Value(std::move(device_value)));
  }

  std::string json_string;
  if (!base::JSONWriter::Write(device_list_value, &json_string)) {
    return nil;
  }

  return base::SysUTF8ToNSString(json_string);
}

- (void)resetSync {
  _worker->ResetSync();
}

- (void)setDidJoinSyncChain:(void (^)(bool success))completion {
  _worker->SetJoinSyncChainCallback(base::BindOnce(completion));
}

- (void)permanentlyDeleteAccount:
    (void (^)(HnsSyncAPISyncProtocolErrorResult))completion {
  _worker->PermanentlyDeleteAccount(base::BindOnce(
      [](void (^completion)(HnsSyncAPISyncProtocolErrorResult),
         const syncer::SyncProtocolError& error) {
        completion(
            static_cast<HnsSyncAPISyncProtocolErrorResult>(error.error_type));
      },
      completion));
}

- (bool)isSyncAccountDeletedNoticePending {
  hns_sync::Prefs hns_sync_prefs(_chromeBrowserState->GetPrefs());
  return hns_sync_prefs.IsSyncAccountDeletedNoticePending();
}

- (void)setIsSyncAccountDeletedNoticePending:
    (bool)isSyncAccountDeletedNoticePending {
  hns_sync::Prefs hns_sync_prefs(_chromeBrowserState->GetPrefs());
  hns_sync_prefs.SetSyncAccountDeletedNoticePending(false);
}

- (bool)isFailedDecryptSeedNoticeDismissed {
  hns_sync::Prefs hns_sync_prefs(_chromeBrowserState->GetPrefs());
  return hns_sync_prefs.IsFailedDecryptSeedNoticeDismissed();
}

- (void)dismissFailedDecryptSeedNotice {
  hns_sync::Prefs hns_sync_prefs(_chromeBrowserState->GetPrefs());
  hns_sync_prefs.DismissFailedDecryptSeedNotice();
}

- (void)deleteDevice:(NSString*)guid {
  _worker->DeleteDevice(base::SysNSStringToUTF8(guid));
}

- (HnsSyncInternalsController*)createSyncInternalsController {
  return [[HnsSyncInternalsController alloc]
      initWithBrowserState:_chromeBrowserState];
}

- (id)createSyncDeviceObserver:(void (^)())onDeviceInfoChanged {
  auto* tracker =
      DeviceInfoSyncServiceFactory::GetForBrowserState(_chromeBrowserState)
          ->GetDeviceInfoTracker();
  return [[HnsSyncDeviceObserver alloc]
      initWithDeviceInfoTracker:tracker
                       callback:onDeviceInfoChanged];
}

- (id)createSyncServiceObserver:(void (^)())onSyncServiceStateChanged
          onSyncServiceShutdown:(void (^)())onSyncServiceShutdown {
  auto* service = static_cast<syncer::SyncServiceImpl*>(
      SyncServiceFactory::GetForBrowserState(_chromeBrowserState));
  return [[HnsSyncServiceObserver alloc]
      initWithSyncServiceImpl:service
         stateChangedCallback:onSyncServiceStateChanged
         syncShutdownCallback:onSyncServiceShutdown];
}
@end
