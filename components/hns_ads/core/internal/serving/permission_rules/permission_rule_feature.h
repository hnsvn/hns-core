/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_ads {

BASE_DECLARE_FEATURE(kPermissionRulesFeature);

bool IsPermissionRuleFeatureEnabled();

constexpr base::FeatureParam<bool> kShouldOnlyServeAdsInWindowedMode{
    &kPermissionRulesFeature, "should_only_serve_ads_in_windowed_mode", true};

constexpr base::FeatureParam<bool>
    kShouldOnlyServeAdsWithValidInternetConnection{
        &kPermissionRulesFeature,
        "should_only_serve_ads_with_valid_internet_connection", true};

constexpr base::FeatureParam<bool> kShouldOnlyServeAdsIfMediaIsNotPlaying{
    &kPermissionRulesFeature, "should_only_serve_ads_if_media_is_not_playing",
    true};

constexpr base::FeatureParam<bool> kShouldOnlyServeAdsIfBrowserIsActive{
    &kPermissionRulesFeature, "should_only_serve_ads_if_browser_is_active",
    true};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_FEATURE_H_
