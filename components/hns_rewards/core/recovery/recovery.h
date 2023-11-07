/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_RECOVERY_RECOVERY_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_RECOVERY_RECOVERY_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/recovery/recovery_empty_balance.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace recovery {

class Recovery {
 public:
  explicit Recovery(RewardsEngineImpl& engine);
  ~Recovery();

  void Check();

 private:
  const raw_ref<RewardsEngineImpl> engine_;
  EmptyBalance empty_balance_;
};

}  // namespace recovery
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_RECOVERY_RECOVERY_H_
