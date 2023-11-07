/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_event_info.h"

#include <tuple>

namespace hns_ads {

bool operator==(const UserActivityEventInfo& lhs,
                const UserActivityEventInfo& rhs) {
  const auto tie = [](const UserActivityEventInfo& user_activity_event) {
    return std::tie(user_activity_event.type, user_activity_event.created_at);
  };

  return tie(lhs) == tie(rhs);
}

bool operator!=(const UserActivityEventInfo& lhs,
                const UserActivityEventInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace hns_ads
