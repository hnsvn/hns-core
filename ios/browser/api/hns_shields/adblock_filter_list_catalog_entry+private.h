/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_PRIVATE_H_
#define HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_PRIVATE_H_

#import <Foundation/Foundation.h>
#include "hns/ios/browser/api/hns_shields/adblock_filter_list_catalog_entry.h"

namespace hns_shields {
class FilterListCatalogEntry;
}  // namespace hns_shields

NS_ASSUME_NONNULL_BEGIN

@interface AdblockFilterListCatalogEntry (Private)
@property(readonly) NSString* base64PublicKey;

- (instancetype)initWithFilterListCatalogEntry:
    (hns_shields::FilterListCatalogEntry)entry;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_PRIVATE_H_
