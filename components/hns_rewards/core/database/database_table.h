/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_TABLE_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_TABLE_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace database {

using ContributionPublisherInfoPair =
    std::pair<std::string, mojom::PublisherInfoPtr>;

using ServerPublisherLinksCallback =
    std::function<void(const std::map<std::string, std::string>& links)>;

using ServerPublisherAmountsCallback =
    std::function<void(const std::vector<double>& amounts)>;

using ContributionQueuePublishersListCallback =
    std::function<void(std::vector<mojom::ContributionQueuePublisherPtr>)>;

using ContributionPublisherListCallback =
    std::function<void(std::vector<mojom::ContributionPublisherPtr>)>;

using ContributionPublisherPairListCallback =
    std::function<void(std::vector<ContributionPublisherInfoPair>)>;

class DatabaseTable {
 public:
  explicit DatabaseTable(RewardsEngineImpl& engine);
  virtual ~DatabaseTable();

 protected:
  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_TABLE_H_
