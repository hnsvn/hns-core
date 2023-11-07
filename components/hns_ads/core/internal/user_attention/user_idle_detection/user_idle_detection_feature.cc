/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_idle_detection/user_idle_detection_feature.h"

namespace hns_ads {

BASE_FEATURE(kUserIdleDetectionFeature,
             "UserIdleDetection",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool IsUserIdleDetectionFeatureEnabled() {
  return base::FeatureList::IsEnabled(kUserIdleDetectionFeature);
}

}  // namespace hns_ads
