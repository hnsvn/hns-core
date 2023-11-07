/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_API_H_
#define HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_API_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class HnsSyncInternalsController;

typedef NSInteger HnsSyncAPISyncProtocolErrorResult
    NS_TYPED_ENUM NS_SWIFT_NAME(HnsSyncAPI.SyncProtocolErrorResult);

OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultSuccess;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultNotMyBirthday;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultThrottled;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultClearPending;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultTransientError;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultMigrationDone;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultDisabledByAdmin;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultPartialFailure;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultDataObsolete;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultEncryptionObsolete;
OBJC_EXPORT HnsSyncAPISyncProtocolErrorResult const
    HnsSyncAPISyncProtocolErrorResultUnknown;

typedef NSInteger HnsSyncAPIQrCodeDataValidationResult NS_TYPED_ENUM
    NS_SWIFT_NAME(HnsSyncAPI.QrCodeDataValidationResult);

OBJC_EXPORT HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultValid;
OBJC_EXPORT HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultNotWellFormed;
OBJC_EXPORT HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultVersionDeprecated;
OBJC_EXPORT HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultExpired;
OBJC_EXPORT HnsSyncAPIQrCodeDataValidationResult const
    HnsSyncAPIQrCodeDataValidationResultValidForTooLong;

typedef NSInteger HnsSyncAPIWordsValidationStatus NS_TYPED_ENUM
    NS_SWIFT_NAME(HnsSyncAPI.WordsValidationStatus);

OBJC_EXPORT HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusValid;
OBJC_EXPORT HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusNotValidPureWords;
OBJC_EXPORT HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusVersionDeprecated;
OBJC_EXPORT HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusExpired;
OBJC_EXPORT HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusValidForTooLong;
OBJC_EXPORT HnsSyncAPIWordsValidationStatus const
    HnsSyncAPIWordsValidationStatusWrongWordsNumber;

OBJC_EXPORT
@interface HnsSyncAPI : NSObject

@property(nonatomic, readonly) bool canSyncFeatureStart;
@property(nonatomic, readonly) bool isSyncFeatureActive;
@property(nonatomic, readonly) bool isInitialSyncFeatureSetupComplete;
@property(nonatomic) bool isSyncAccountDeletedNoticePending;
@property(nonatomic, readonly) bool isFailedDecryptSeedNoticeDismissed;

- (instancetype)init NS_UNAVAILABLE;

- (void)requestSync;

- (void)setSetupComplete;

- (void)resetSync;

- (void)setDidJoinSyncChain:(void (^)(bool))completion;

- (void)permanentlyDeleteAccount:
    (void (^)(HnsSyncAPISyncProtocolErrorResult))completion;

- (void)deleteDevice:(NSString*)guid;

- (bool)isValidSyncCode:(NSString*)syncCode;

- (NSString*)getSyncCode;

// returns false is sync is already configured or if the sync code is invalid
- (bool)setSyncCode:(NSString*)syncCode;

- (NSString*)syncCodeFromHexSeed:(NSString*)hexSeed;

- (NSString*)hexSeedFromSyncCode:(NSString*)syncCode;

- (NSString*)qrCodeJsonFromHexSeed:(NSString*)hexSeed;

- (void)dismissFailedDecryptSeedNotice;

- (HnsSyncAPIQrCodeDataValidationResult)getQRCodeValidationResult:
    (NSString*)json;

- (HnsSyncAPIWordsValidationStatus)getWordsValidationResult:
    (NSString*)timeLimitedWords;

- (NSString*)getWordsFromTimeLimitedWords:(NSString*)timeLimitedWords;

- (NSString*)getTimeLimitedWordsFromWords:(NSString*)words;

- (NSString*)getHexSeedFromQrCodeJson:(NSString*)json;

- (nullable UIImage*)getQRCodeImage:(CGSize)size;

- (nullable NSString*)getDeviceListJSON;

- (HnsSyncInternalsController*)createSyncInternalsController;

- (id)createSyncDeviceObserver:(void (^)())onDeviceInfoChanged;
- (id)createSyncServiceObserver:(void (^)())onSyncServiceStateChanged
          onSyncServiceShutdown:(void (^)())onSyncServiceShutdown;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_API_H_
