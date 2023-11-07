/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/wallet_provider/uphold/get_uphold_wallet.h"

#include "hns/components/hns_rewards/core/global_constants.h"

namespace hns_rewards::internal::uphold {

GetUpholdWallet::GetUpholdWallet(RewardsEngineImpl& engine)
    : wallet_provider::GetExternalWallet(engine) {}

GetUpholdWallet::~GetUpholdWallet() = default;

const char* GetUpholdWallet::WalletType() const {
  return constant::kWalletUphold;
}

}  // namespace hns_rewards::internal::uphold
