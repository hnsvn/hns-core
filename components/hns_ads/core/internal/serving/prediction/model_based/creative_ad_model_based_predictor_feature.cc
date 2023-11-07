/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor_feature.h"  // IWYU pragma: keep

namespace hns_ads {

BASE_FEATURE(CreativeAdModelBasedPredictorFeature,
             "CreativeAdModelBasedPredictor",
             base::FEATURE_ENABLED_BY_DEFAULT);

}  // namespace hns_ads
