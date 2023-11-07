/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_trigger_info.h"

#include <limits>

#include "base/numerics/ranges.h"

namespace hns_ads {

bool operator==(const UserActivityTriggerInfo& lhs,
                const UserActivityTriggerInfo& rhs) {
  return lhs.event_sequence == rhs.event_sequence &&
         base::IsApproximatelyEqual(lhs.score, rhs.score,
                                    std::numeric_limits<double>::epsilon());
}

bool operator!=(const UserActivityTriggerInfo& lhs,
                const UserActivityTriggerInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace hns_ads
