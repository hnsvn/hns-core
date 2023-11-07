/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WELCOME_COMMON_FEATURES_H_
#define HNS_COMPONENTS_HNS_WELCOME_COMMON_FEATURES_H_

#include "base/feature_list.h"

namespace hns_welcome {
namespace features {

// If enabled, this will show the Hns Rewards card in onboarding
BASE_DECLARE_FEATURE(kShowRewardsCard);

}  // namespace features
}  // namespace hns_welcome

#endif  // HNS_COMPONENTS_HNS_WELCOME_COMMON_FEATURES_H_
