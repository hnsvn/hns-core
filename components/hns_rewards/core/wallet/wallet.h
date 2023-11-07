/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_H_

#include <stdint.h>

#include <string>

#include "base/containers/flat_map.h"
#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/endpoint/promotion/promotion_server.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "hns/components/hns_rewards/core/wallet/wallet_balance.h"
#include "hns/components/hns_rewards/core/wallet/wallet_create.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace wallet {

class Wallet {
 public:
  explicit Wallet(RewardsEngineImpl& engine);
  ~Wallet();

  void CreateWalletIfNecessary(absl::optional<std::string>&& geo_country,
                               CreateRewardsWalletCallback callback);

  void FetchBalance(FetchBalanceCallback callback);

  mojom::RewardsWalletPtr GetWallet();
  mojom::RewardsWalletPtr GetWallet(bool* corrupted);

  bool SetWallet(mojom::RewardsWalletPtr wallet);

 private:
  const raw_ref<RewardsEngineImpl> engine_;
  WalletCreate create_;
  WalletBalance balance_;
  endpoint::PromotionServer promotion_server_;
};

}  // namespace wallet
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_H_
