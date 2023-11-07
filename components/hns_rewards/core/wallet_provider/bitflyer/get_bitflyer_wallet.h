/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_GET_BITFLYER_WALLET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_GET_BITFLYER_WALLET_H_

#include "hns/components/hns_rewards/core/wallet_provider/get_external_wallet.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace bitflyer {

class GetBitFlyerWallet : public wallet_provider::GetExternalWallet {
 public:
  explicit GetBitFlyerWallet(RewardsEngineImpl& engine);

  ~GetBitFlyerWallet() override;

 private:
  const char* WalletType() const override;
};

}  // namespace bitflyer
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_GET_BITFLYER_WALLET_H_
