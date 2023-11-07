/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V31_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V31_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

// Migration 31 adds a contribution processor field to the pending_contribution
// table so that we do not attempt to retry contributions in publisher/user
// wallet mismatch scenarios.
const char v31[] = R"(
  ALTER TABLE pending_contribution ADD processor INTEGER DEFAULT 0 NOT NULL;
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V31_H_
