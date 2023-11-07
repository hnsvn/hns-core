/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_OPENTABS_HNS_OPENTABS_API_PRIVATE_H_
#define HNS_IOS_BROWSER_API_OPENTABS_HNS_OPENTABS_API_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "hns/ios/browser/api/opentabs/hns_opentabs_api.h"

NS_ASSUME_NONNULL_BEGIN

namespace syncer {
class SyncService;
}  // namespace syncer

namespace sync_sessions {
class SessionSyncService;
}  // namespace sync_sessions

@interface HnsOpenTabsAPI (Private)
- (instancetype)initWithSyncService:(syncer::SyncService*)syncService
                 sessionSyncService:
                     (sync_sessions::SessionSyncService*)sessionSyncService;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_OPENTABS_HNS_OPENTABS_API_PRIVATE_H_
