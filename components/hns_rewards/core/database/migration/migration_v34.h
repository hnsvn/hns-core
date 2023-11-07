/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V34_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V34_H_

namespace hns_rewards::internal {
namespace database {
namespace migration {

// Migration 34 adds a "claimable_until" field to the promotion database so that
// a "days to claim" countdown can be displayed to the user.
const char v34[] = R"(
  ALTER TABLE promotion ADD COLUMN claimable_until INTEGER;
)";

}  // namespace migration
}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V34_H_
