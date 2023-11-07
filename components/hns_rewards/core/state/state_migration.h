/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "hns/components/hns_rewards/core/state/state_migration_v1.h"
#include "hns/components/hns_rewards/core/state/state_migration_v10.h"
#include "hns/components/hns_rewards/core/state/state_migration_v11.h"
#include "hns/components/hns_rewards/core/state/state_migration_v12.h"
#include "hns/components/hns_rewards/core/state/state_migration_v13.h"
#include "hns/components/hns_rewards/core/state/state_migration_v2.h"
#include "hns/components/hns_rewards/core/state/state_migration_v3.h"
#include "hns/components/hns_rewards/core/state/state_migration_v4.h"
#include "hns/components/hns_rewards/core/state/state_migration_v5.h"
#include "hns/components/hns_rewards/core/state/state_migration_v6.h"
#include "hns/components/hns_rewards/core/state/state_migration_v7.h"
#include "hns/components/hns_rewards/core/state/state_migration_v8.h"
#include "hns/components/hns_rewards/core/state/state_migration_v9.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace state {

class StateMigration {
 public:
  explicit StateMigration(RewardsEngineImpl& engine);
  ~StateMigration();

  void Start(ResultCallback callback);

  void Migrate(ResultCallback callback);

 private:
  void FreshInstall(ResultCallback callback);

  void OnMigration(ResultCallback callback, int version, mojom::Result result);

  const raw_ref<RewardsEngineImpl> engine_;
  StateMigrationV1 v1_;
  StateMigrationV2 v2_;
  StateMigrationV3 v3_;
  StateMigrationV4 v4_;
  StateMigrationV5 v5_;
  StateMigrationV6 v6_;
  StateMigrationV7 v7_;
  StateMigrationV8 v8_;
  StateMigrationV9 v9_;
  StateMigrationV10 v10_;
  StateMigrationV11 v11_;
  StateMigrationV12 v12_;
  StateMigrationV13 v13_;
};

}  // namespace state
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_H_
