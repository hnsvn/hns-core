/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_CONTRIBUTION_INFO_PUBLISHERS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_CONTRIBUTION_INFO_PUBLISHERS_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

class DatabaseContributionInfoPublishers : public DatabaseTable {
 public:
  explicit DatabaseContributionInfoPublishers(RewardsEngineImpl& engine);
  ~DatabaseContributionInfoPublishers() override;

  void InsertOrUpdate(mojom::DBTransaction* transaction,
                      mojom::ContributionInfoPtr info);

  void GetRecordByContributionList(
      const std::vector<std::string>& contribution_ids,
      ContributionPublisherListCallback callback);

  void GetContributionPublisherPairList(
      const std::vector<std::string>& contribution_ids,
      ContributionPublisherPairListCallback callback);

  void UpdateContributedAmount(const std::string& contribution_id,
                               const std::string& publisher_key,
                               LegacyResultCallback callback);

 private:
  void OnGetRecordByContributionList(ContributionPublisherListCallback callback,
                                     mojom::DBCommandResponsePtr response);

  void OnGetContributionPublisherInfoMap(
      ContributionPublisherPairListCallback callback,
      mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_CONTRIBUTION_INFO_PUBLISHERS_H_
