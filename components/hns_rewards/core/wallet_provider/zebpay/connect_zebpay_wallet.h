/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_ZEBPAY_CONNECT_ZEBPAY_WALLET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_ZEBPAY_CONNECT_ZEBPAY_WALLET_H_

#include "hns/components/hns_rewards/core/endpoints/zebpay/post_oauth_zebpay.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "hns/components/hns_rewards/core/wallet_provider/connect_external_wallet.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace zebpay {

class ConnectZebPayWallet : public wallet_provider::ConnectExternalWallet {
 public:
  explicit ConnectZebPayWallet(RewardsEngineImpl& engine);

  ~ConnectZebPayWallet() override;

 private:
  const char* WalletType() const override;

  void Authorize(OAuthInfo&& oauth_info,
                 ConnectExternalWalletCallback callback) override;

  void OnAuthorize(ConnectExternalWalletCallback callback,
                   endpoints::PostOAuthZebPay::Result&& result) const;
};

}  // namespace zebpay
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_ZEBPAY_CONNECT_ZEBPAY_WALLET_H_
