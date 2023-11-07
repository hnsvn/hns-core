/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_SYNC_SERVICE_SYNC_AUTH_MANAGER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_SYNC_SERVICE_SYNC_AUTH_MANAGER_H_

namespace syncer {
class HnsSyncAuthManager;
}  // namespace syncer

// Header guard to prevent DetermineAccountToUse from getting overriden in it
#include "components/sync/service/sync_auth_manager.h"

#define RequestAccessToken virtual RequestAccessToken
#define DetermineAccountToUse      \
  DetermineAccountToUse_Unused() { \
    return SyncAccountInfo();      \
  }                                \
  friend HnsSyncAuthManager;     \
  virtual SyncAccountInfo DetermineAccountToUse

#include "src/components/sync/service/sync_auth_manager.h"  // IWYU pragma: export

#undef RequestAccessToken
#undef DetermineAccountToUse
#endif  // HNS_CHROMIUM_SRC_COMPONENTS_SYNC_SERVICE_SYNC_AUTH_MANAGER_H_
