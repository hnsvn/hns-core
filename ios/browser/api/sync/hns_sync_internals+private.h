/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_INTERNALS_PRIVATE_H_
#define HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_INTERNALS_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "hns/ios/browser/api/sync/hns_sync_internals.h"

NS_ASSUME_NONNULL_BEGIN

class ChromeBrowserState;

@interface HnsSyncInternalsController (Private)
- (instancetype)initWithBrowserState:(ChromeBrowserState*)mainBrowserState;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_INTERNALS_PRIVATE_H_
