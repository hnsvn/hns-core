/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SERVER_PUBLISHER_INFO_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SERVER_PUBLISHER_INFO_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_server_publisher_banner.h"
#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

using GetServerPublisherInfoCallback =
    std::function<void(mojom::ServerPublisherInfoPtr)>;

class DatabaseServerPublisherInfo : public DatabaseTable {
 public:
  explicit DatabaseServerPublisherInfo(RewardsEngineImpl& engine);
  ~DatabaseServerPublisherInfo() override;

  void InsertOrUpdate(const mojom::ServerPublisherInfo& server_info,
                      LegacyResultCallback callback);

  void GetRecord(const std::string& publisher_key,
                 GetServerPublisherInfoCallback callback);

  void DeleteExpiredRecords(int64_t max_age_seconds,
                            LegacyResultCallback callback);

 private:
  void OnGetRecordBanner(mojom::PublisherBannerPtr banner,
                         const std::string& publisher_key,
                         GetServerPublisherInfoCallback callback);

  void OnGetRecord(GetServerPublisherInfoCallback callback,
                   const std::string& publisher_key,
                   mojom::PublisherBannerPtr banner,
                   mojom::DBCommandResponsePtr response);

  void OnExpiredRecordsSelected(LegacyResultCallback callback,
                                mojom::DBCommandResponsePtr response);

  DatabaseServerPublisherBanner banner_;
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SERVER_PUBLISHER_INFO_H_
