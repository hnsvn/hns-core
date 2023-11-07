/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_API_API_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_API_API_H_

#include "hns/components/hns_rewards/core/api/api_parameters.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace api {

class API {
 public:
  explicit API(RewardsEngineImpl& engine);
  ~API();

  void Initialize();

  void FetchParameters(GetRewardsParametersCallback callback);

 private:
  APIParameters parameters_;
};

}  // namespace api
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_API_API_H_
