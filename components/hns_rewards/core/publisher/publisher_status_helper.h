/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_PUBLISHER_PUBLISHER_STATUS_HELPER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_PUBLISHER_PUBLISHER_STATUS_HELPER_H_

#include <vector>

#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace publisher {

// Refreshes the publisher status for each entry in the specified list
void RefreshPublisherStatus(RewardsEngineImpl& engine,
                            std::vector<mojom::PublisherInfoPtr>&& info_list,
                            GetRecurringTipsCallback callback);

}  // namespace publisher
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_PUBLISHER_PUBLISHER_STATUS_HELPER_H_
