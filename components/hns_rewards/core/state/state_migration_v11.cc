/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/state/state_migration_v11.h"

#include "base/check.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "hns/components/hns_rewards/core/state/state.h"
#include "hns/components/hns_rewards/core/state/state_keys.h"

namespace hns_rewards::internal {
namespace state {

StateMigrationV11::StateMigrationV11(RewardsEngineImpl& engine)
    : engine_(engine) {}

StateMigrationV11::~StateMigrationV11() = default;

void StateMigrationV11::Migrate(LegacyResultCallback callback) {
  // In version 7 encryption was added for |kWalletHns|. However due to wallet
  // corruption, users copying their profiles to new computers or reinstalling
  // their operating system we are reverting this change

  const auto decrypted_wallet =
      engine_->state()->GetEncryptedString(kWalletHns);
  if (decrypted_wallet) {
    engine_->SetState(kWalletHns, decrypted_wallet.value());
  }

  callback(mojom::Result::OK);
}

}  // namespace state
}  // namespace hns_rewards::internal
