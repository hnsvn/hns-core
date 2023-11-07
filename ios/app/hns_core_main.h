/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_APP_HNS_CORE_MAIN_H_
#define HNS_IOS_APP_HNS_CORE_MAIN_H_

#import <Foundation/Foundation.h>

#import "hns_core_switches.h"  // NOLINT

@class HnsBookmarksAPI;
@class HnsHistoryAPI;
@class HnsPasswordAPI;
@class HnsOpenTabsAPI;
@class HnsP3AUtils;
@class HnsSendTabAPI;
@class HnsSyncAPI;
@class HnsSyncProfileServiceIOS;
@class HnsStats;
@class HnsWalletAPI;
@class AdblockService;
@class HnsTabGeneratorAPI;
@class WebImageDownloader;
@class NTPBackgroundImagesService;
@protocol IpfsAPI;

NS_ASSUME_NONNULL_BEGIN

typedef int HnsCoreLogSeverity NS_TYPED_ENUM;
OBJC_EXPORT const HnsCoreLogSeverity HnsCoreLogSeverityFatal;
OBJC_EXPORT const HnsCoreLogSeverity HnsCoreLogSeverityError;
OBJC_EXPORT const HnsCoreLogSeverity HnsCoreLogSeverityWarning;
OBJC_EXPORT const HnsCoreLogSeverity HnsCoreLogSeverityInfo;
OBJC_EXPORT const HnsCoreLogSeverity HnsCoreLogSeverityVerbose;

typedef bool (^HnsCoreLogHandler)(HnsCoreLogSeverity severity,
                                    NSString* file,
                                    int line,
                                    size_t messageStart,
                                    NSString* formattedMessage);

OBJC_EXPORT
@interface HnsCoreMain : NSObject

@property(nonatomic, readonly) HnsBookmarksAPI* bookmarksAPI;

@property(nonatomic, readonly) HnsHistoryAPI* historyAPI;

@property(nonatomic, readonly) HnsPasswordAPI* passwordAPI;

@property(nonatomic, readonly) HnsOpenTabsAPI* openTabsAPI;

@property(nonatomic, readonly) HnsSendTabAPI* sendTabAPI;

@property(nonatomic, readonly) HnsSyncAPI* syncAPI;

@property(nonatomic, readonly) HnsSyncProfileServiceIOS* syncProfileService;

@property(nonatomic, readonly) HnsTabGeneratorAPI* tabGeneratorAPI;

@property(nonatomic, readonly) WebImageDownloader* webImageDownloader;

/// Sets the global log handler for Chromium & HnsCore logs.
///
/// When a custom log handler is set, it is the responsibility of the client
/// to handle fatal logs from CHECK (and DCHECK on debug builds) by checking
/// the `serverity` passed in.
+ (void)setLogHandler:(nullable HnsCoreLogHandler)logHandler;

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithUserAgent:(NSString*)userAgent;

- (instancetype)initWithUserAgent:(NSString*)userAgent
               additionalSwitches:
                   (NSArray<HnsCoreSwitch*>*)additionalSwitches;

- (void)scheduleLowPriorityStartupTasks;

@property(readonly) HnsWalletAPI* hnsWalletAPI;

@property(readonly) HnsStats* hnsStats;

@property(readonly) AdblockService* adblockService;

@property(readonly) id<IpfsAPI> ipfsAPI;

- (void)initializeP3AServiceForChannel:(NSString*)channel
                         weekOfInstall:(NSString*)weekOfInstall;

@property(readonly) HnsP3AUtils* p3aUtils;

@property(readonly) NTPBackgroundImagesService* backgroundImagesService;

/// Sets up bundle path overrides and initializes ICU from the HnsCore bundle
/// without setting up a HnsCoreMain instance.
///
/// Should only be called in unit tests
+ (bool)initializeICUForTesting;

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_APP_HNS_CORE_MAIN_H_
