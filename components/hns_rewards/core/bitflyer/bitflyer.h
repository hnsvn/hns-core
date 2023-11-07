/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_BITFLYER_BITFLYER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_BITFLYER_BITFLYER_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/common/mojom/rewards.mojom.h"
#include "hns/components/hns_rewards/core/endpoint/bitflyer/bitflyer_server.h"
#include "hns/components/hns_rewards/core/wallet_provider/wallet_provider.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace bitflyer {

class Bitflyer final : public wallet_provider::WalletProvider {
 public:
  explicit Bitflyer(RewardsEngineImpl& engine);

  const char* WalletType() const override;

  void FetchBalance(
      base::OnceCallback<void(mojom::Result, double)> callback) override;

  std::string GetFeeAddress() const override;

 private:
  endpoint::BitflyerServer server_;
};

}  // namespace bitflyer
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_BITFLYER_BITFLYER_H_
