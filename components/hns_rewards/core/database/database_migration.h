/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MIGRATION_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MIGRATION_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace database {

class DatabaseMigration {
 public:
  explicit DatabaseMigration(RewardsEngineImpl& engine);
  ~DatabaseMigration();

  void Start(uint32_t table_version, ResultCallback callback);

  static void SetTargetVersionForTesting(uint32_t version);

 private:
  void GenerateCommand(mojom::DBTransaction* transaction,
                       const std::string& query);

  void RunDBTransactionCallback(ResultCallback callback,
                                uint32_t start_version,
                                int migrated_version,
                                mojom::DBCommandResponsePtr response);

  const raw_ref<RewardsEngineImpl> engine_;
  static uint32_t test_target_version_;
};

}  // namespace database
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MIGRATION_H_
