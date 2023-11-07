/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "hns/components/hns_rewards/core/state/state_migration_v8.h"

#include <string>

#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "hns/components/hns_rewards/core/state/state_keys.h"

namespace hns_rewards::internal {
namespace state {

StateMigrationV8::StateMigrationV8(RewardsEngineImpl& engine)
    : engine_(engine) {}

StateMigrationV8::~StateMigrationV8() = default;

void StateMigrationV8::Migrate(LegacyResultCallback callback) {
  const bool enabled = engine_->GetState<bool>("enabled");

  if (!enabled) {
    engine_->SetState(kAutoContributeEnabled, false);
  }

  callback(mojom::Result::OK);
}

}  // namespace state
}  // namespace hns_rewards::internal
