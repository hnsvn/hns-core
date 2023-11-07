/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/purchase_intent_feature.h"

namespace hns_ads {

BASE_FEATURE(kPurchaseIntentFeature,
             "PurchaseIntent",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool IsPurchaseIntentFeatureEnabled() {
  return base::FeatureList::IsEnabled(kPurchaseIntentFeature);
}

}  // namespace hns_ads
