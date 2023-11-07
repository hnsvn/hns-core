/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SYNC_TEST_HNS_MOCK_SYNC_ENGINE_H_
#define HNS_COMPONENTS_SYNC_TEST_HNS_MOCK_SYNC_ENGINE_H_

#include "base/functional/callback_forward.h"
#include "components/sync/test/mock_sync_engine.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace syncer {

struct SyncProtocolError;

class HnsMockSyncEngine : public MockSyncEngine {
 public:
  HnsMockSyncEngine();
  ~HnsMockSyncEngine() override;

  MOCK_METHOD(void,
              PermanentlyDeleteAccount,
              (base::OnceCallback<void(const SyncProtocolError&)>),
              (override));
};

}  // namespace syncer

#endif  // HNS_COMPONENTS_SYNC_TEST_HNS_MOCK_SYNC_ENGINE_H_
