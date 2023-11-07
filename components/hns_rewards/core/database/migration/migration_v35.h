/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V35_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V35_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

// Migration 35 removes the server_publisher_amounts table; now that we support
// custom tip amounts, we no longer need to support creator-provided default
// amounts.
const char v35[] = R"(
  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS server_publisher_amounts;
  PRAGMA foreign_keys = on;
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V35_H_
