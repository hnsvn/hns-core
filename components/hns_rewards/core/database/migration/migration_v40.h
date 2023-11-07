/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V40_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V40_H_

namespace hns_rewards::internal::database::migration {

// Migration 40 removes the pending contributions table.
constexpr char v40[] = R"sql(
  DROP TABLE IF EXISTS pending_contribution;
  DROP TABLE IF EXISTS processed_publisher;
)sql";

}  // namespace hns_rewards::internal::database::migration

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V40_H_
