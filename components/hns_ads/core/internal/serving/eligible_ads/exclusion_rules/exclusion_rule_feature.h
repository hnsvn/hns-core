/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULE_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULE_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

BASE_DECLARE_FEATURE(kExclusionRulesFeature);

bool IsExclusionRuleFeatureEnabled();

constexpr base::FeatureParam<bool> kShouldExcludeAdIfConverted{
    &kExclusionRulesFeature, "should_exclude_ad_if_converted", true};

constexpr base::FeatureParam<base::TimeDelta>
    kShouldExcludeAdIfDismissedWithinTimeWindow{
        &kExclusionRulesFeature,
        "should_exclude_ad_if_dismissed_within_time_window", base::Hours(0)};

constexpr base::FeatureParam<base::TimeDelta>
    kShouldExcludeAdIfTransferredWithinTimeWindow{
        &kExclusionRulesFeature,
        "should_exclude_ad_if_transferred_within_time_window", base::Hours(0)};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULE_FEATURE_H_
