/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/wallet_provider/zebpay/get_zebpay_wallet.h"

#include "hns/components/hns_rewards/core/global_constants.h"

namespace hns_rewards::internal::zebpay {

GetZebPayWallet::GetZebPayWallet(RewardsEngineImpl& engine)
    : wallet_provider::GetExternalWallet(engine) {}

GetZebPayWallet::~GetZebPayWallet() = default;

const char* GetZebPayWallet::WalletType() const {
  return constant::kWalletZebPay;
}

}  // namespace hns_rewards::internal::zebpay
