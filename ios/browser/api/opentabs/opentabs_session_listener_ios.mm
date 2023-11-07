/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/opentabs/opentabs_session_listener_ios.h"

#include "base/check.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_observer.h"

#include "hns/ios/browser/api/opentabs/hns_opentabs_api.h"
#include "hns/ios/browser/api/opentabs/hns_opentabs_observer.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace hns {
namespace ios {

OpenTabsSessionListenerIOS::OpenTabsSessionListenerIOS(
    id<OpenTabsSessionStateObserver> observer,
    syncer::SyncService* service)
    : observer_(observer), service_(service) {
  DCHECK(observer_);
  DCHECK(service_);
  service_->AddObserver(this);
}

OpenTabsSessionListenerIOS::~OpenTabsSessionListenerIOS() {
  DCHECK(service_);
  service_->RemoveObserver(this);
}

void OpenTabsSessionListenerIOS::OnStateChanged(syncer::SyncService* sync) {
  if ([observer_ respondsToSelector:@selector(openTabsSyncStateChanged)]) {
    [observer_ openTabsSyncStateChanged];
  }
}

void OpenTabsSessionListenerIOS::OnSyncCycleCompleted(
    syncer::SyncService* sync) {
  if ([observer_ respondsToSelector:@selector(openTabsSyncCycleCompleted)]) {
    [observer_ openTabsSyncCycleCompleted];
  }
}

void OpenTabsSessionListenerIOS::OnSyncConfigurationCompleted(
    syncer::SyncService* sync) {
  if ([observer_
          respondsToSelector:@selector(openTabsSyncConfigurationCompleted)]) {
    [observer_ openTabsSyncConfigurationCompleted];
  }
}

void OpenTabsSessionListenerIOS::OnSyncShutdown(syncer::SyncService* sync) {
  if ([observer_ respondsToSelector:@selector(openTabsSyncShutdown)]) {
    [observer_ openTabsSyncShutdown];
  }
}

}  // namespace ios
}  // namespace hns

@interface OpenTabsSessionListenerImpl () {
  std::unique_ptr<hns::ios::OpenTabsSessionListenerIOS> observer_;
  syncer::SyncService* sync_service_;
}
@end

@implementation OpenTabsSessionListenerImpl
- (instancetype)init:(id<OpenTabsSessionStateObserver>)observer
         syncService:(void*)service {
  if ((self = [super init])) {
    observer_ = std::make_unique<hns::ios::OpenTabsSessionListenerIOS>(
        observer, static_cast<syncer::SyncService*>(service));

    sync_service_ = static_cast<syncer::SyncService*>(service);
  }
  return self;
}

- (void)dealloc {
  [self destroy];
}

- (void)destroy {
  observer_.reset();
}
@end
