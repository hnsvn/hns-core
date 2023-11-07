/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/user_activity_permission_rule_unittest_util.h"

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"

namespace hns_ads {

void ForceUserActivityPermissionRuleForTesting() {
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);
}

}  // namespace hns_ads
