/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SYNC_ENGINE_HNS_SYNC_SERVER_COMMANDS_H_
#define HNS_COMPONENTS_SYNC_ENGINE_HNS_SYNC_SERVER_COMMANDS_H_

#include "base/functional/callback_forward.h"
#include "components/sync/protocol/sync_protocol_error.h"

namespace syncer {

class SyncCycle;
struct SyncProtocolError;

class HnsSyncServerCommands {
 public:
  HnsSyncServerCommands(const HnsSyncServerCommands&) = delete;
  HnsSyncServerCommands& operator=(const HnsSyncServerCommands&) = delete;

  static void PermanentlyDeleteAccount(
      SyncCycle* cycle,
      base::OnceCallback<void(const SyncProtocolError&)> callback);

 private:
  HnsSyncServerCommands() = default;
};

}  // namespace syncer

#endif  // HNS_COMPONENTS_SYNC_ENGINE_HNS_SYNC_SERVER_COMMANDS_H_
