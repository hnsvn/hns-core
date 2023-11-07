/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MEDIA_PUBLISHER_INFO_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MEDIA_PUBLISHER_INFO_H_

#include <string>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

class DatabaseMediaPublisherInfo : public DatabaseTable {
 public:
  explicit DatabaseMediaPublisherInfo(RewardsEngineImpl& engine);
  ~DatabaseMediaPublisherInfo() override;

  void InsertOrUpdate(const std::string& media_key,
                      const std::string& publisher_key,
                      LegacyResultCallback callback);

  void GetRecord(const std::string& media_key, PublisherInfoCallback callback);

 private:
  void OnGetRecord(PublisherInfoCallback callback,
                   mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MEDIA_PUBLISHER_INFO_H_
