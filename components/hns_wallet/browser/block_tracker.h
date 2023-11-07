/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_BLOCK_TRACKER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_BLOCK_TRACKER_H_

#include <map>
#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

namespace hns_wallet {

class JsonRpcService;

class BlockTracker {
 public:
  explicit BlockTracker(JsonRpcService* json_rpc_service);
  virtual ~BlockTracker();

  virtual void Start(const std::string& chain_id, base::TimeDelta interval) = 0;
  virtual void Stop(const std::string& chain_id);
  virtual void Stop();
  bool IsRunning(const std::string& chain_id) const;

 protected:
  // <chain_id, timer>
  std::map<std::string, std::unique_ptr<base::RepeatingTimer>> timers_;
  raw_ptr<JsonRpcService> json_rpc_service_ = nullptr;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_BLOCK_TRACKER_H_
