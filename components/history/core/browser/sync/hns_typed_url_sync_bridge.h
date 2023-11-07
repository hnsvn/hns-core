/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HISTORY_CORE_BROWSER_SYNC_HNS_TYPED_URL_SYNC_BRIDGE_H_
#define HNS_COMPONENTS_HISTORY_CORE_BROWSER_SYNC_HNS_TYPED_URL_SYNC_BRIDGE_H_

#include <memory>

#include "components/history/core/browser/sync/typed_url_sync_bridge.h"

namespace history {

class HnsTypedURLSyncBridge : public TypedURLSyncBridge {
 public:
  HnsTypedURLSyncBridge(
      HistoryBackend* history_backend,
      TypedURLSyncMetadataDatabase* sync_metadata_store,
      std::unique_ptr<syncer::ModelTypeChangeProcessor> change_processor);

  HnsTypedURLSyncBridge(const HnsTypedURLSyncBridge&) = delete;
  HnsTypedURLSyncBridge& operator=(const HnsTypedURLSyncBridge&) = delete;

  ~HnsTypedURLSyncBridge() override = default;
  bool ShouldSyncVisit(const URLRow& url_row,
                       ui::PageTransition transition) override;

  static int GetSendAllFlagVisitThrottleThreshold();
  static int GetSendAllFlagVisitThrottleMultiple();

 private:
  friend class HnsTypedURLSyncBridgeTest;
};

}  // namespace history

#endif  // HNS_COMPONENTS_HISTORY_CORE_BROWSER_SYNC_HNS_TYPED_URL_SYNC_BRIDGE_H_
