/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SERVER_PUBLISHER_LINKS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SERVER_PUBLISHER_LINKS_H_

#include <map>
#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

class DatabaseServerPublisherLinks : public DatabaseTable {
 public:
  explicit DatabaseServerPublisherLinks(RewardsEngineImpl& engine);
  ~DatabaseServerPublisherLinks() override;

  void InsertOrUpdate(mojom::DBTransaction* transaction,
                      const mojom::ServerPublisherInfo& server_info);

  void DeleteRecords(mojom::DBTransaction* transaction,
                     const std::string& publisher_key_list);

  void GetRecord(const std::string& publisher_key,
                 ServerPublisherLinksCallback callback);

 private:
  void OnGetRecord(ServerPublisherLinksCallback callback,
                   mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SERVER_PUBLISHER_LINKS_H_
