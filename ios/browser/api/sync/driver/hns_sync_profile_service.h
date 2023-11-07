/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_SYNC_DRIVER_HNS_SYNC_PROFILE_SERVICE_H_
#define HNS_IOS_BROWSER_API_SYNC_DRIVER_HNS_SYNC_PROFILE_SERVICE_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, HnsSyncUserSelectableTypes) {
  HnsSyncUserSelectableTypes_NONE = 1ULL << 0,
  HnsSyncUserSelectableTypes_BOOKMARKS = 1ULL << 1,
  HnsSyncUserSelectableTypes_PREFERENCES = 1ULL << 2,
  HnsSyncUserSelectableTypes_PASSWORDS = 1ULL << 3,
  HnsSyncUserSelectableTypes_AUTOFILL = 1ULL << 4,
  HnsSyncUserSelectableTypes_THEMES = 1ULL << 5,
  HnsSyncUserSelectableTypes_HISTORY = 1ULL << 6,
  HnsSyncUserSelectableTypes_EXTENSIONS = 1ULL << 7,
  HnsSyncUserSelectableTypes_APPS = 1ULL << 8,
  HnsSyncUserSelectableTypes_READING_LIST = 1ULL << 9,
  HnsSyncUserSelectableTypes_TABS = 1ULL << 10,
};

OBJC_EXPORT
@interface HnsSyncProfileServiceIOS : NSObject

- (instancetype)init NS_UNAVAILABLE;

// Whether all conditions are satisfied for Sync to start
// Does not imply that Sync is actually running
@property(nonatomic, readonly) bool isSyncFeatureActive;

/// Selectable Types for the Sync User
/// Used for opting in/out on iOS side
@property(nonatomic, assign) HnsSyncUserSelectableTypes userSelectedTypes;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_SYNC_DRIVER_HNS_SYNC_PROFILE_SERVICE_H_
