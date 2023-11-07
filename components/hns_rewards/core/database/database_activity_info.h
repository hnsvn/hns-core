/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_ACTIVITY_INFO_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_ACTIVITY_INFO_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

class DatabaseActivityInfo : public DatabaseTable {
 public:
  explicit DatabaseActivityInfo(RewardsEngineImpl& engine);
  ~DatabaseActivityInfo() override;

  void InsertOrUpdate(mojom::PublisherInfoPtr info,
                      LegacyResultCallback callback);

  void NormalizeList(std::vector<mojom::PublisherInfoPtr> list,
                     LegacyResultCallback callback);

  void GetRecordsList(const int start,
                      const int limit,
                      mojom::ActivityInfoFilterPtr filter,
                      GetActivityInfoListCallback callback);

  void DeleteRecord(const std::string& publisher_key,
                    LegacyResultCallback callback);

  void GetPublishersVisitedCount(base::OnceCallback<void(int)> callback);

 private:
  void CreateInsertOrUpdate(mojom::DBTransaction* transaction,
                            mojom::PublisherInfoPtr info);

  void OnNormalizeList(LegacyResultCallback callback,
                       std::vector<mojom::PublisherInfoPtr> list,
                       mojom::DBCommandResponsePtr response);

  void OnGetRecordsList(GetActivityInfoListCallback callback,
                        mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_ACTIVITY_INFO_H_
