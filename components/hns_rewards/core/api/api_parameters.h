/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_API_API_PARAMETERS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_API_API_PARAMETERS_H_

#include <memory>
#include <vector>

#include "base/memory/raw_ref.h"
#include "base/timer/timer.h"
#include "hns/components/hns_rewards/core/endpoints/hns/get_parameters.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace api {

class APIParameters {
 public:
  explicit APIParameters(RewardsEngineImpl& engine);
  ~APIParameters();

  void Initialize();

  void Fetch(GetRewardsParametersCallback callback);

 private:
  void OnFetch(endpoints::GetParameters::Result&&);

  void RunCallbacks();

  void SetRefreshTimer(base::TimeDelta delay,
                       base::TimeDelta base_delay = base::TimeDelta());

  const raw_ref<RewardsEngineImpl> engine_;
  base::OneShotTimer refresh_timer_;
  std::vector<GetRewardsParametersCallback> callbacks_;
};

}  // namespace api
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_API_API_PARAMETERS_H_
