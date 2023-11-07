/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_BITFLYER_BITFLYER_SERVER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_BITFLYER_BITFLYER_SERVER_H_

#include "hns/components/hns_rewards/core/endpoint/bitflyer/get_balance/get_balance_bitflyer.h"
#include "hns/components/hns_rewards/core/endpoint/bitflyer/post_oauth/post_oauth_bitflyer.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {

class BitflyerServer {
 public:
  explicit BitflyerServer(RewardsEngineImpl& engine);
  ~BitflyerServer();

  bitflyer::GetBalance& get_balance() { return get_balance_; }

  bitflyer::PostOauth& post_oauth() { return post_oauth_; }

 private:
  bitflyer::GetBalance get_balance_;
  bitflyer::PostOauth post_oauth_;
};

}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_BITFLYER_BITFLYER_SERVER_H_
