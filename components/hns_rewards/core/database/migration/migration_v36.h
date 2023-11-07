/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V36_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V36_H_

namespace hns_rewards::internal::database::migration {

// Migration 36 converts all stored publisher status values of "CONNECTED" to
// "NOT_VERIFIED".
const char v36[] = R"(
  UPDATE server_publisher_info SET status = 0 WHERE status = 1;
)";

}  // namespace hns_rewards::internal::database::migration

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_MIGRATION_MIGRATION_V36_H_
