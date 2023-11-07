/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_CREATE_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_CREATE_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace wallet {

class WalletCreate {
 public:
  explicit WalletCreate(RewardsEngineImpl& engine);

  void CreateWallet(absl::optional<std::string>&& geo_country,
                    CreateRewardsWalletCallback callback);

 private:
  template <typename Result>
  void OnResult(CreateRewardsWalletCallback,
                absl::optional<std::string>&& geo_country,
                Result&&);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace wallet
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_CREATE_H_
