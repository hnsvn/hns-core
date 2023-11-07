/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V33_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V33_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

// Migration 33 removes the contribution processor field
// from the pending_contribution table.
// We'll resolve the processor at the time
// the pending contribution is being retried.
const char v33[] = R"(
  ALTER TABLE pending_contribution DROP COLUMN processor;
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V33_H_
