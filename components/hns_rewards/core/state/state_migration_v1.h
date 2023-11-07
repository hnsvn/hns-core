/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V1_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V1_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/legacy/publisher_state.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace state {

class StateMigrationV1 {
 public:
  explicit StateMigrationV1(RewardsEngineImpl& engine);
  ~StateMigrationV1();

  void Migrate(LegacyResultCallback callback);

  bool legacy_data_migrated() const { return legacy_data_migrated_; }

 private:
  void OnLoadState(mojom::Result result, LegacyResultCallback callback);

  void BalanceReportsSaved(mojom::Result result, LegacyResultCallback callback);

  std::unique_ptr<publisher::LegacyPublisherState> legacy_publisher_;
  const raw_ref<RewardsEngineImpl> engine_;
  bool legacy_data_migrated_ = false;
};

}  // namespace state
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V1_H_
