/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection_feature.h"

namespace hns_ads {

bool MaybeScreenWasLocked(const bool screen_was_locked) {
  return kShouldDetectScreenWasLocked.Get() && screen_was_locked;
}

bool HasExceededMaximumIdleTime(const base::TimeDelta idle_time) {
  const base::TimeDelta maximum_idle_time = kMaximumIdleTime.Get();
  if (maximum_idle_time.is_zero()) {  // Infinite
    return false;
  }

  return idle_time > maximum_idle_time;
}

}  // namespace hns_ads
