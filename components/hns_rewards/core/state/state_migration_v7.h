/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V7_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V7_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace state {

class StateMigrationV7 {
 public:
  explicit StateMigrationV7(RewardsEngineImpl& engine);
  ~StateMigrationV7();

  void Migrate(LegacyResultCallback callback);

 private:
  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace state
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V7_H_
