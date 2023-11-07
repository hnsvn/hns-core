/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_CONNECT_EXTERNAL_WALLET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_CONNECT_EXTERNAL_WALLET_H_

#include <string>

#include "base/containers/flat_map.h"
#include "base/memory/raw_ref.h"
#include "base/timer/timer.h"
#include "base/types/expected.h"
#include "hns/components/hns_rewards/core/endpoints/hns/get_wallet.h"
#include "hns/components/hns_rewards/core/endpoints/common/post_connect.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace wallet_provider {

class ConnectExternalWallet {
 public:
  explicit ConnectExternalWallet(RewardsEngineImpl& engine);

  virtual ~ConnectExternalWallet();

  void Run(const base::flat_map<std::string, std::string>& query_parameters,
           ConnectExternalWalletCallback);

 protected:
  virtual const char* WalletType() const = 0;

  struct OAuthInfo {
    std::string one_time_string, code_verifier, code;
  };

  virtual void Authorize(OAuthInfo&&, ConnectExternalWalletCallback) = 0;

  void OnConnect(ConnectExternalWalletCallback,
                 std::string&& token,
                 std::string&& address,
                 std::string&& country_id,
                 endpoints::PostConnect::Result&&) const;

  const raw_ref<RewardsEngineImpl> engine_;

 private:
  absl::optional<OAuthInfo> ExchangeOAuthInfo(mojom::ExternalWalletPtr) const;

  base::expected<std::string, mojom::ConnectExternalWalletError> GetCode(
      const base::flat_map<std::string, std::string>& query_parameters,
      const std::string& current_one_time_string) const;

  void CheckLinkage();

  void CheckLinkageCallback(endpoints::GetWallet::Result&& result);

  base::RetainingOneShotTimer linkage_checker_;
};

}  // namespace wallet_provider
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_CONNECT_EXTERNAL_WALLET_H_
