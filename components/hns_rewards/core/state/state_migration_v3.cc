/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/state/state_migration_v3.h"

namespace hns_rewards::internal {
namespace state {

StateMigrationV3::StateMigrationV3() = default;

StateMigrationV3::~StateMigrationV3() = default;

void StateMigrationV3::Migrate(LegacyResultCallback callback) {
  // In this migration we migrated anon address to uphold wallet in preferences
  // because anon address was removed we can also remove this step
  // Ref: https://github.com/hnsvn/hns-browser/issues/11150
  callback(mojom::Result::OK);
}

}  // namespace state
}  // namespace hns_rewards::internal
