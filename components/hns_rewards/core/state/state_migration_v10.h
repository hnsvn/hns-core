/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V10_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V10_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/endpoints/hns/get_wallet.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace state {

class StateMigrationV10 {
 public:
  explicit StateMigrationV10(RewardsEngineImpl& engine);
  ~StateMigrationV10();

  void Migrate(LegacyResultCallback callback);

 private:
  void OnGetWallet(LegacyResultCallback callback,
                   endpoints::GetWallet::Result&& result);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace state
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V10_H_
