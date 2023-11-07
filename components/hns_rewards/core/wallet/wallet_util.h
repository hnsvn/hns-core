/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_UTIL_H_

#include <set>
#include <string>

#include "hns/components/hns_rewards/common/mojom/rewards.mojom.h"
#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"
#include "third_party/abseil-cpp/absl/types/variant.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace wallet {

mojom::ExternalWalletPtr GetWallet(RewardsEngineImpl& engine,
                                   const std::string& wallet_type);

mojom::ExternalWalletPtr GetWalletIf(RewardsEngineImpl& engine,
                                     const std::string& wallet_type,
                                     const std::set<mojom::WalletStatus>&);

bool SetWallet(RewardsEngineImpl& engine, mojom::ExternalWalletPtr);

mojom::ExternalWalletPtr TransitionWallet(
    RewardsEngineImpl& engine,
    absl::variant<mojom::ExternalWalletPtr, std::string> wallet_info,
    mojom::WalletStatus to);

mojom::ExternalWalletPtr MaybeCreateWallet(RewardsEngineImpl& engine,
                                           const std::string& wallet_type);

bool LogOutWallet(RewardsEngineImpl& engine,
                  const std::string& wallet_type,
                  const std::string& notification = "");

mojom::ExternalWalletPtr GenerateLinks(mojom::ExternalWalletPtr);

void FetchBalance(RewardsEngineImpl& engine,
                  const std::string& wallet_type,
                  base::OnceCallback<void(mojom::Result, double)> callback);

}  // namespace wallet
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_WALLET_UTIL_H_
