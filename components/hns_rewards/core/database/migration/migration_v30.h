/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V30_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V30_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

// On migration away from "BAP" (a Japan-specific presentation of BAT), copy
// the data in the unblinded_tokens table to an archive table and clear the
// unblinded_tokens table.
const char v30[] = R"(
  CREATE TABLE unblinded_tokens_bap AS SELECT * from unblinded_tokens;
  DELETE FROM unblinded_tokens;
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V30_H_
