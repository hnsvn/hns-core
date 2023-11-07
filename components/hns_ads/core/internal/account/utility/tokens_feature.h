/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_TOKENS_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_TOKENS_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_ads {

BASE_DECLARE_FEATURE(kAccountTokensFeature);

constexpr base::FeatureParam<int> kMinConfirmationTokens{
    &kAccountTokensFeature, "min_confirmation_tokens", 20};

constexpr base::FeatureParam<int> kMaxConfirmationTokens{
    &kAccountTokensFeature, "max_confirmation_tokens", 50};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_TOKENS_FEATURE_H_
