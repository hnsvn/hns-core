/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/recovery/recovery.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "hns/components/hns_rewards/core/state/state.h"

namespace hns_rewards::internal {
namespace recovery {

Recovery::Recovery(RewardsEngineImpl& engine)
    : engine_(engine), empty_balance_(engine) {}

Recovery::~Recovery() = default;

void Recovery::Check() {
  if (!engine_->state()->GetEmptyBalanceChecked()) {
    BLOG(1, "Running empty balance check...")
    empty_balance_.Check();
  }
}

}  // namespace recovery
}  // namespace hns_rewards::internal
