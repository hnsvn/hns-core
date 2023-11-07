/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_PURCHASE_INTENT_FEATURE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_PURCHASE_INTENT_FEATURE_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

BASE_DECLARE_FEATURE(kPurchaseIntentFeature);

bool IsPurchaseIntentFeatureEnabled();

constexpr base::FeatureParam<int> kPurchaseIntentResourceVersion{
    &kPurchaseIntentFeature, "resource_version", 1};

constexpr base::FeatureParam<int> kPurchaseIntentThreshold{
    &kPurchaseIntentFeature, "threshold", 3};

constexpr base::FeatureParam<base::TimeDelta> kPurchaseIntentTimeWindow{
    &kPurchaseIntentFeature, "time_window", base::Days(7)};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_PURCHASE_INTENT_FEATURE_H_
