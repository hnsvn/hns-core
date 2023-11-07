// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SHIELDS_COMMON_FEATURES_H_
#define HNS_COMPONENTS_HNS_SHIELDS_COMMON_FEATURES_H_

#include <string>

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_shields {
namespace features {
BASE_DECLARE_FEATURE(kHnsAdblockDefault1pBlocking);
BASE_DECLARE_FEATURE(kHnsAdblockCnameUncloaking);
BASE_DECLARE_FEATURE(kHnsAdblockCollapseBlockedElements);
BASE_DECLARE_FEATURE(kHnsAdblockCookieListDefault);
BASE_DECLARE_FEATURE(kHnsAdblockCookieListOptIn);
BASE_DECLARE_FEATURE(kHnsAdblockCosmeticFiltering);
BASE_DECLARE_FEATURE(kHnsAdblockCspRules);
BASE_DECLARE_FEATURE(kHnsAdblockMobileNotificationsListDefault);
BASE_DECLARE_FEATURE(kHnsAdblockScriptletDebugLogs);
BASE_DECLARE_FEATURE(kHnsDomainBlock);
BASE_DECLARE_FEATURE(kHnsDomainBlock1PES);
BASE_DECLARE_FEATURE(kHnsExtensionNetworkBlocking);
BASE_DECLARE_FEATURE(kHnsReduceLanguage);
BASE_DECLARE_FEATURE(kHnsLocalhostAccessPermission);
BASE_DECLARE_FEATURE(kHnsDarkModeBlock);
BASE_DECLARE_FEATURE(kCosmeticFilteringSyncLoad);
BASE_DECLARE_FEATURE(kCosmeticFilteringExtraPerfMetrics);
BASE_DECLARE_FEATURE(kCosmeticFilteringJsPerformance);
extern const base::FeatureParam<std::string>
    kCosmeticFilteringSubFrameFirstSelectorsPollingDelayMs;
extern const base::FeatureParam<std::string>
    kCosmeticFilteringswitchToSelectorsPollingThreshold;
extern const base::FeatureParam<std::string>
    kCosmeticFilteringFetchNewClassIdRulesThrottlingMs;
BASE_DECLARE_FEATURE(kAdblockOverrideRegexDiscardPolicy);
extern const base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyCleanupIntervalSec;
extern const base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyDiscardUnusedSec;

}  // namespace features
}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_COMMON_FEATURES_H_
