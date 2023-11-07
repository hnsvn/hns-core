/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_MONTHLY_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_MONTHLY_H_

#include <vector>

#include "base/memory/raw_ref.h"
#include "base/time/time.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace contribution {

class ContributionMonthly {
 public:
  explicit ContributionMonthly(RewardsEngineImpl& engine);

  ~ContributionMonthly();

  void Process(absl::optional<base::Time> cutoff_time,
               LegacyResultCallback callback);

 private:
  void AdvanceContributionDates(
      absl::optional<base::Time> cutoff_time,
      LegacyResultCallback callback,
      std::vector<mojom::PublisherInfoPtr> publishers);

  void OnNextContributionDateAdvanced(
      std::vector<mojom::PublisherInfoPtr> publishers,
      LegacyResultCallback callback,
      bool success);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace contribution
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_MONTHLY_H_
