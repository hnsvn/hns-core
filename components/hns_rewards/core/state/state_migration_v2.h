/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V2_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V2_H_

#include <memory>
#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/legacy/bat_state.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace state {

class StateMigrationV2 {
 public:
  explicit StateMigrationV2(RewardsEngineImpl& engine);
  ~StateMigrationV2();

  void Migrate(LegacyResultCallback callback);

 private:
  void OnLoadState(mojom::Result result, LegacyResultCallback callback);

  std::unique_ptr<LegacyBatState> legacy_state_;
  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace state
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V2_H_
