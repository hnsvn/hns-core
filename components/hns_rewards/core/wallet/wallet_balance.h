/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_BALANCE_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_BALANCE_H_

#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace wallet {

class WalletBalance {
 public:
  explicit WalletBalance(RewardsEngineImpl& engine);
  ~WalletBalance();

  void Fetch(FetchBalanceCallback callback);

 private:
  void OnGetUnblindedTokens(FetchBalanceCallback callback,
                            std::vector<mojom::UnblindedTokenPtr> tokens);

  void OnFetchExternalWalletBalance(const std::string& wallet_type,
                                    mojom::BalancePtr balance_ptr,
                                    FetchBalanceCallback callback,
                                    mojom::Result result,
                                    double balance);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace wallet
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_BALANCE_H_
