/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_COMMON_TIME_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_COMMON_TIME_UTIL_H_

#include "base/time/time.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {
namespace util {

mojom::ActivityMonth GetCurrentMonth();

mojom::ActivityMonth GetMonth(const base::Time& time);

uint32_t GetCurrentYear();

uint32_t GetYear(const base::Time& time);

uint64_t GetCurrentTimeStamp();

base::TimeDelta GetRandomizedDelay(base::TimeDelta delay);

base::TimeDelta GetRandomizedDelayWithBackoff(base::TimeDelta delay,
                                              base::TimeDelta max_delay,
                                              int backoff_count);

}  // namespace util
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_COMMON_TIME_UTIL_H_
