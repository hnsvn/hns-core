/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V39_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V39_H_

namespace hns_rewards::internal::database::migration {

// Migration 39 adds a `web3_url` field to the publisher banner table in order
// to support P2P creator contributions.
constexpr char v39[] = R"sql(
  ALTER TABLE server_publisher_banner ADD COLUMN web3_url TEXT;
)sql";

}  // namespace hns_rewards::internal::database::migration

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V39_H_
