/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CONSTANTS_H_

#include "base/time/time.h"

namespace hns_rewards::internal {
namespace constant {

const char kClearFavicon[] = "clear";

const char kIgnorePublisherBlob[] = "ignore";

const uint64_t kReconcileInterval =
    30 * base::Time::kHoursPerDay * base::Time::kSecondsPerHour;

const uint64_t kPromotionRefreshInterval =
    base::Time::kHoursPerDay * base::Time::kSecondsPerHour;

const uint64_t kPendingContributionExpirationInterval =
    90 * base::Time::kHoursPerDay * base::Time::kSecondsPerHour;

const double kVotePrice = 0.25;

const int kMinVisitTime = 8;

}  // namespace constant
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CONSTANTS_H_
