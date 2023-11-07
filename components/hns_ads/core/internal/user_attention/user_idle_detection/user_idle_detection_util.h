/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_IDLE_DETECTION_USER_IDLE_DETECTION_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_IDLE_DETECTION_USER_IDLE_DETECTION_UTIL_H_

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

bool MaybeScreenWasLocked(bool screen_was_locked);

bool HasExceededMaximumIdleTime(base::TimeDelta idle_time);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_IDLE_DETECTION_USER_IDLE_DETECTION_UTIL_H_
