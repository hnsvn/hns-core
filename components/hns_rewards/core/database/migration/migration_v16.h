/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V16_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V16_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

const char v16[] = R"(
  UPDATE contribution_info SET
  created_at = (
    CASE WHEN datetime(created_at, 'unixepoch') IS NULL
    THEN strftime('%s', datetime(created_at))
    ELSE created_at END
  );
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V16_H_
