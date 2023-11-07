/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_AC_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_AC_H_

#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace contribution {

class ContributionAC {
 public:
  explicit ContributionAC(RewardsEngineImpl& engine);

  ~ContributionAC();

  void Process(const uint64_t reconcile_stamp);

 private:
  void PreparePublisherList(std::vector<mojom::PublisherInfoPtr> list);

  void QueueSaved(const mojom::Result result);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace contribution
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_AC_H_
