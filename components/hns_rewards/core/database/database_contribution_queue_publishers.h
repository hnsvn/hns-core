/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_CONTRIBUTION_QUEUE_PUBLISHERS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_CONTRIBUTION_QUEUE_PUBLISHERS_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

class DatabaseContributionQueuePublishers : public DatabaseTable {
 public:
  explicit DatabaseContributionQueuePublishers(RewardsEngineImpl& engine);
  ~DatabaseContributionQueuePublishers() override;

  void InsertOrUpdate(const std::string& id,
                      std::vector<mojom::ContributionQueuePublisherPtr> list,
                      LegacyResultCallback callback);

  void GetRecordsByQueueId(const std::string& queue_id,
                           ContributionQueuePublishersListCallback callback);

 private:
  void OnGetRecordsByQueueId(ContributionQueuePublishersListCallback callback,
                             mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_CONTRIBUTION_QUEUE_PUBLISHERS_H_
