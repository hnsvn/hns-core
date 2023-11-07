/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_GET_EXTERNAL_WALLET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_GET_EXTERNAL_WALLET_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace wallet_provider {

class GetExternalWallet {
 public:
  explicit GetExternalWallet(RewardsEngineImpl& engine);

  virtual ~GetExternalWallet();

  void Run(GetExternalWalletCallback) const;

 protected:
  virtual const char* WalletType() const = 0;

  virtual void OnTransferTokens(GetExternalWalletCallback,
                                mojom::Result,
                                std::string drain_id) const;

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace wallet_provider
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_GET_EXTERNAL_WALLET_H_
