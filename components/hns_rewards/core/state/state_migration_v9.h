/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V9_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V9_H_

#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal::state {

class StateMigrationV9 {
 public:
  StateMigrationV9();
  ~StateMigrationV9();

  void Migrate(LegacyResultCallback callback);
};

}  // namespace hns_rewards::internal::state

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_STATE_STATE_MIGRATION_V9_H_
