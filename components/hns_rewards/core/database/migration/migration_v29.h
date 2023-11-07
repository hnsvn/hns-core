/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V29_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V29_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

const char v29[] = R"(
  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS event_log;
  PRAGMA foreign_keys = on;

  CREATE TABLE event_log (
    event_log_id LONGVARCHAR PRIMARY KEY NOT NULL,
    key TEXT NOT NULL,
    value TEXT NOT NULL,
    created_at TIMESTAMP NOT NULL
  );
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V29_H_
