/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/common/features.h"

#include "base/feature_list.h"

namespace hns_rewards {
namespace features {

#if BUILDFLAG(IS_ANDROID)
//  Flag for Hns Rewards.
#if defined(ARCH_CPU_X86_FAMILY) && defined(OFFICIAL_BUILD)
BASE_FEATURE(kHnsRewards, "HnsRewards", base::FEATURE_DISABLED_BY_DEFAULT);
#else
BASE_FEATURE(kHnsRewards, "HnsRewards", base::FEATURE_ENABLED_BY_DEFAULT);
#endif
#endif  // BUILDFLAG(IS_ANDROID)

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
BASE_FEATURE(kGeminiFeature,
             "HnsRewardsGemini",
             base::FEATURE_ENABLED_BY_DEFAULT);
#endif

BASE_FEATURE(kVBatNoticeFeature,
             "HnsRewardsVBatNotice",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kVerboseLoggingFeature,
             "HnsRewardsVerboseLogging",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kAllowUnsupportedWalletProvidersFeature,
             "HnsRewardsAllowUnsupportedWalletProviders",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features
}  // namespace hns_rewards
