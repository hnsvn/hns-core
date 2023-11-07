/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_COMMON_FEATURES_H_
#define HNS_COMPONENTS_HNS_REWARDS_COMMON_FEATURES_H_

#include "base/feature_list.h"
#include "hns/components/hns_rewards/common/buildflags/buildflags.h"
#include "build/build_config.h"

namespace hns_rewards {
namespace features {

#if BUILDFLAG(IS_ANDROID)
BASE_DECLARE_FEATURE(kHnsRewards);
#endif  // BUILDFLAG(IS_ANDROID)

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
BASE_DECLARE_FEATURE(kGeminiFeature);
#endif

BASE_DECLARE_FEATURE(kVBatNoticeFeature);

BASE_DECLARE_FEATURE(kVerboseLoggingFeature);

BASE_DECLARE_FEATURE(kAllowUnsupportedWalletProvidersFeature);

}  // namespace features
}  // namespace hns_rewards

#endif  // HNS_COMPONENTS_HNS_REWARDS_COMMON_FEATURES_H_
