/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_REWARDS_REWARDS_UTIL_H_
#define HNS_BROWSER_HNS_REWARDS_REWARDS_UTIL_H_

#include "hns/components/hns_rewards/common/rewards_util.h"

class Profile;

namespace hns_rewards {

bool IsSupportedForProfile(
    Profile* profile,
    IsSupportedOptions options = IsSupportedOptions::kNone);

}  // namespace hns_rewards

#endif  // HNS_BROWSER_HNS_REWARDS_REWARDS_UTIL_H_
