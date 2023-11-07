/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/wallet_provider/bitflyer/get_bitflyer_wallet.h"

#include "hns/components/hns_rewards/core/global_constants.h"

namespace hns_rewards::internal::bitflyer {

GetBitFlyerWallet::GetBitFlyerWallet(RewardsEngineImpl& engine)
    : wallet_provider::GetExternalWallet(engine) {}

GetBitFlyerWallet::~GetBitFlyerWallet() = default;

const char* GetBitFlyerWallet::WalletType() const {
  return constant::kWalletBitflyer;
}

}  // namespace hns_rewards::internal::bitflyer
