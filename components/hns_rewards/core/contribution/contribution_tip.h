/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_TIP_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_TIP_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal {

class RewardsEngineImpl;

namespace contribution {

class ContributionTip {
 public:
  explicit ContributionTip(RewardsEngineImpl& engine);

  ~ContributionTip();

  using ProcessCallback = base::OnceCallback<void(absl::optional<std::string>)>;

  void Process(const std::string& publisher_id,
               double amount,
               ProcessCallback callback);

 private:
  void OnPublisherDataRead(const std::string& publisher_id,
                           double amount,
                           ProcessCallback callback,
                           mojom::ServerPublisherInfoPtr server_info);

  void OnQueueSaved(const std::string& queue_id,
                    ProcessCallback callback,
                    mojom::Result result);

  void OnPendingTipSaved(mojom::Result result);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace contribution
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_TIP_H_
