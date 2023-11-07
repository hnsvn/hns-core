/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_TRIGGER_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_TRIGGER_INFO_H_

#include <string>
#include <vector>

namespace hns_ads {

struct UserActivityTriggerInfo final {
  std::string event_sequence;
  double score = 0.0;
};

bool operator==(const UserActivityTriggerInfo&, const UserActivityTriggerInfo&);
bool operator!=(const UserActivityTriggerInfo&, const UserActivityTriggerInfo&);

using UserActivityTriggerList = std::vector<UserActivityTriggerInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_TRIGGER_INFO_H_
