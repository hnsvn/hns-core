/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_IOS_CHROME_BROWSER_SYNC_IOS_CHROME_SYNC_CLIENT_H__
#define HNS_CHROMIUM_SRC_IOS_CHROME_BROWSER_SYNC_IOS_CHROME_SYNC_CLIENT_H__

class IOSChromeSyncClient;
using IOSChromeSyncClient_HnsImpl = IOSChromeSyncClient;

#define IOSChromeSyncClient IOSChromeSyncClient_ChromiumImpl
#define component_factory_ \
  component_factory_;      \
  friend IOSChromeSyncClient_HnsImpl

#include "src/ios/chrome/browser/sync/ios_chrome_sync_client.h"  // IWYU pragma: export
#undef component_factory_
#undef IOSChromeSyncClient

class IOSChromeSyncClient : public IOSChromeSyncClient_ChromiumImpl {
 public:
  using IOSChromeSyncClient_ChromiumImpl::IOSChromeSyncClient_ChromiumImpl;

  IOSChromeSyncClient(const IOSChromeSyncClient&) = delete;
  IOSChromeSyncClient& operator=(const IOSChromeSyncClient&) = delete;

  ~IOSChromeSyncClient() override;

  // BrowserSyncClient implementation.
  syncer::DataTypeController::TypeVector CreateDataTypeControllers(
      syncer::SyncService* sync_service) override;
};

#endif  // HNS_CHROMIUM_SRC_IOS_CHROME_BROWSER_SYNC_IOS_CHROME_SYNC_CLIENT_H__
