/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_EMBEDDING_BASED_CREATIVE_AD_EMBEDDING_BASED_PREDICTOR_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_EMBEDDING_BASED_CREATIVE_AD_EMBEDDING_BASED_PREDICTOR_UTIL_H_

#include <vector>

namespace hns_ads {

double CalculateNormalizingConstantForVoteRegistry(
    const std::vector<int>& creative_ad_vote_registry);

std::vector<double> ComputeCreativeAdProbabilitiesForVoteRegistry(
    const std::vector<int>& creative_ad_vote_registry);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PREDICTION_EMBEDDING_BASED_CREATIVE_AD_EMBEDDING_BASED_PREDICTOR_UTIL_H_
