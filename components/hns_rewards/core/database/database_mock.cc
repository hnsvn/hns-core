/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/database/database_mock.h"

namespace hns_rewards::internal {
namespace database {

MockDatabase::MockDatabase(RewardsEngineImpl& engine) : Database(engine) {}

MockDatabase::~MockDatabase() = default;

}  // namespace database
}  // namespace hns_rewards::internal
