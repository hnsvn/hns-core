/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_CONNECT_BITFLYER_WALLET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_CONNECT_BITFLYER_WALLET_H_

#include <string>

#include "base/containers/flat_map.h"
#include "hns/components/hns_rewards/core/endpoint/bitflyer/bitflyer_server.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "hns/components/hns_rewards/core/wallet_provider/connect_external_wallet.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace bitflyer {

class ConnectBitFlyerWallet : public wallet_provider::ConnectExternalWallet {
 public:
  explicit ConnectBitFlyerWallet(RewardsEngineImpl& engine);

  ~ConnectBitFlyerWallet() override;

 private:
  const char* WalletType() const override;

  void Authorize(OAuthInfo&&, ConnectExternalWalletCallback) override;

  void OnAuthorize(ConnectExternalWalletCallback,
                   mojom::Result,
                   std::string&& token,
                   std::string&& address,
                   std::string&& linking_info) const;

  endpoint::BitflyerServer bitflyer_server_;
};

}  // namespace bitflyer
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_CONNECT_BITFLYER_WALLET_H_
