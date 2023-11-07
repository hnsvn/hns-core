/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_EVENT_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_EVENT_INFO_H_

#include "base/containers/circular_deque.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_event_types.h"

namespace hns_ads {

struct UserActivityEventInfo final {
  UserActivityEventType type;
  base::Time created_at;
};

bool operator==(const UserActivityEventInfo&, const UserActivityEventInfo&);
bool operator!=(const UserActivityEventInfo&, const UserActivityEventInfo&);

using UserActivityEventList = base::circular_deque<UserActivityEventInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_EVENT_INFO_H_
