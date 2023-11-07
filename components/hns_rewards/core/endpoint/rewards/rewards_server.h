/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_REWARDS_REWARDS_SERVER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_REWARDS_REWARDS_SERVER_H_

#include "hns/components/hns_rewards/core/endpoint/rewards/get_prefix_list/get_prefix_list.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {

class RewardsServer {
 public:
  explicit RewardsServer(RewardsEngineImpl& engine);
  ~RewardsServer();

  rewards::GetPrefixList& get_prefix_list() { return get_prefix_list_; }

 private:
  rewards::GetPrefixList get_prefix_list_;
};

}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_REWARDS_REWARDS_SERVER_H_
