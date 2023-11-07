// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/components/hns_shields/common/features.h"

#include "base/feature_list.h"

namespace hns_shields {
namespace features {

// When enabled, Hns will block first-party requests that appear in a filter
// list when Shields is in "standard" blocking mode. When disabled, Hns will
// allow first-party requests in "standard" blocking mode regardless of whether
// or not they appear in a filter list.
BASE_FEATURE(kHnsAdblockDefault1pBlocking,
             "HnsAdblockDefault1pBlocking",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, Hns will issue DNS queries for requests that the adblock
// engine has not blocked, then check them again with the original hostname
// substituted for any canonical name found.
BASE_FEATURE(kHnsAdblockCnameUncloaking,
             "HnsAdblockCnameUncloaking",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Hns will apply HTML element collapsing to all images and
// iframes that initiate a blocked network request.
BASE_FEATURE(kHnsAdblockCollapseBlockedElements,
             "HnsAdblockCollapseBlockedElements",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Hns will enable "Easylist-Cookie List" by default unless
// overridden by a locally set preference.
BASE_FEATURE(kHnsAdblockCookieListDefault,
             "HnsAdblockCookieListDefault",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Hns will display a bubble inviting the user to turn on the
// "Easylist-Cookie List" filter.
BASE_FEATURE(kHnsAdblockCookieListOptIn,
             "HnsAdblockCookieListOptIn",
             base::FEATURE_DISABLED_BY_DEFAULT);
BASE_FEATURE(kHnsAdblockCosmeticFiltering,
             "HnsAdblockCosmeticFiltering",
             base::FEATURE_ENABLED_BY_DEFAULT);
BASE_FEATURE(kHnsAdblockScriptletDebugLogs,
             "HnsAdblockScriptletDebugLogs",
             base::FEATURE_DISABLED_BY_DEFAULT);
BASE_FEATURE(kHnsAdblockCspRules,
             "HnsAdblockCspRules",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Hns will enable "Fanboy's Mobile Notifications List" by
// default unless overridden by a locally set preference.
BASE_FEATURE(kHnsAdblockMobileNotificationsListDefault,
             "HnsAdblockMobileNotificationsListDefault",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, Hns will block domains listed in the user's selected adblock
// filters and present a security interstitial with choice to proceed and
// optionally whitelist the domain.
// Domain block filters look like this:
// ||ads.example.com^
BASE_FEATURE(kHnsDomainBlock,
             "HnsDomainBlock",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, Hns will attempt to enable 1PES mode in a standard blocking
// mode when a user visists a domain that is present in currently active adblock
// filters. 1PES will be enabled only if neither cookies nor localStorage data
// is stored for the website.
BASE_FEATURE(kHnsDomainBlock1PES,
             "HnsDomainBlock1PES",
             base::FEATURE_ENABLED_BY_DEFAULT);
// When enabled, network requests initiated by extensions will be checked and
// potentially blocked by Hns Shields.
BASE_FEATURE(kHnsExtensionNetworkBlocking,
             "HnsExtensionNetworkBlocking",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, language headers and APIs may be altered by Hns Shields.
BASE_FEATURE(kHnsReduceLanguage,
             "HnsReduceLanguage",
             base::FEATURE_ENABLED_BY_DEFAULT);
// when enabled, hns will prompt for permission on sites which want to connect
// to localhost.
BASE_FEATURE(kHnsLocalhostAccessPermission,
             "HnsLocalhostAccessPermission",
             base::FEATURE_DISABLED_BY_DEFAULT);
// When enabled, Hns will always report Light in Fingerprinting: Strict mode
BASE_FEATURE(kHnsDarkModeBlock,
             "HnsDarkModeBlock",
             base::FEATURE_ENABLED_BY_DEFAULT);
// load the cosmetic filter rules using sync ipc
BASE_FEATURE(kCosmeticFilteringSyncLoad,
             "CosmeticFilterSyncLoad",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Enables extra TRACE_EVENTs in content filter js. The feature is
// primary designed for local debugging.
BASE_FEATURE(kCosmeticFilteringExtraPerfMetrics,
             "CosmeticFilteringExtraPerfMetrics",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kCosmeticFilteringJsPerformance,
             "CosmeticFilteringJsPerformance",
             base::FEATURE_ENABLED_BY_DEFAULT);

constexpr base::FeatureParam<std::string>
    kCosmeticFilteringSubFrameFirstSelectorsPollingDelayMs{
        &kCosmeticFilteringJsPerformance, "subframes_first_query_delay_ms",
        "1000"};

constexpr base::FeatureParam<std::string>
    kCosmeticFilteringswitchToSelectorsPollingThreshold{
        &kCosmeticFilteringJsPerformance, "switch_to_polling_threshold",
        "1000"};

constexpr base::FeatureParam<std::string>
    kCosmeticFilteringFetchNewClassIdRulesThrottlingMs{
        &kCosmeticFilteringJsPerformance, "fetch_throttling_ms", "100"};

BASE_FEATURE(kAdblockOverrideRegexDiscardPolicy,
             "AdblockOverrideRegexDiscardPolicy",
             base::FEATURE_DISABLED_BY_DEFAULT);

constexpr base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyCleanupIntervalSec{
        &kAdblockOverrideRegexDiscardPolicy, "cleanup_interval_sec", 0};

constexpr base::FeatureParam<int>
    kAdblockOverrideRegexDiscardPolicyDiscardUnusedSec{
        &kAdblockOverrideRegexDiscardPolicy, "discard_unused_sec", 180};

}  // namespace features
}  // namespace hns_shields
