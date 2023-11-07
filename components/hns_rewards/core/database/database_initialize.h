/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_INITIALIZE_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_INITIALIZE_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/database/database_migration.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace database {

class DatabaseInitialize {
 public:
  explicit DatabaseInitialize(RewardsEngineImpl& engine);
  ~DatabaseInitialize();

  void Start(ResultCallback callback);

 private:
  void OnInitialize(ResultCallback callback,
                    mojom::DBCommandResponsePtr response);

  const raw_ref<RewardsEngineImpl> engine_;
  DatabaseMigration migration_;
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_INITIALIZE_H_
