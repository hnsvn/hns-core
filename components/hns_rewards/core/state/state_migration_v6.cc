/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/state/state_migration_v6.h"

#include <map>
#include <string>
#include <utility>

#include "base/json/json_writer.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "hns/components/hns_rewards/core/state/state_keys.h"

namespace hns_rewards::internal {
namespace state {

StateMigrationV6::StateMigrationV6(RewardsEngineImpl& engine)
    : engine_(engine) {}

StateMigrationV6::~StateMigrationV6() = default;

void StateMigrationV6::Migrate(LegacyResultCallback callback) {
  auto uphold_wallet = engine_->GetLegacyWallet();
  engine_->SetState(kWalletUphold, uphold_wallet);
  engine_->client()->ClearState("external_wallets");

  base::Value::Dict hns;
  hns.Set("payment_id", engine_->GetState<std::string>(kPaymentId));
  hns.Set("recovery_seed", engine_->GetState<std::string>(kRecoverySeed));

  std::string hns_json;
  base::JSONWriter::Write(hns, &hns_json);
  engine_->SetState(kWalletHns, std::move(hns_json));

  callback(mojom::Result::OK);
}

}  // namespace state
}  // namespace hns_rewards::internal
