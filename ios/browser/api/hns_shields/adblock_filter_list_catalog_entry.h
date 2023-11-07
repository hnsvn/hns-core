/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_H_
#define HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface AdblockFilterListCatalogEntry : NSObject
@property(readonly) NSString* uuid;
@property(readonly) NSString* url;
@property(readonly) NSString* title;
@property(readonly) NSArray<NSString*>* languages;
@property(readonly) NSString* supportURL;
@property(readonly) NSString* componentId;
@property(readonly) NSString* base64PublicKey;
@property(readonly) NSString* desc;
- (instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_H_
