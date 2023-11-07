/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_news/common/features.h"

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_news::features {

BASE_FEATURE(kHnsNewsCardPeekFeature,
             "HnsNewsCardPeek",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsNewsFeedUpdate,
             "HnsNewsFeedUpdate",
             base::FEATURE_DISABLED_BY_DEFAULT);
const base::FeatureParam<int> kHnsNewsMinBlockCards{&kHnsNewsFeedUpdate,
                                                      "min-block-cards", 1};

const base::FeatureParam<int> kHnsNewsMaxBlockCards{&kHnsNewsFeedUpdate,
                                                      "max-block-cards", 5};

const base::FeatureParam<double> kHnsNewsPopScoreHalfLife{
    &kHnsNewsFeedUpdate, "pop-score-half-life", 18};
const base::FeatureParam<double> kHnsNewsPopScoreFallback{
    &kHnsNewsFeedUpdate, "pop-score-fallback", 50};

const base::FeatureParam<double> kHnsNewsInlineDiscoveryRatio{
    &kHnsNewsFeedUpdate, "inline-discovery-ratio", 0.25};

const base::FeatureParam<double> kHnsNewsSourceSubscribedMin{
    &kHnsNewsFeedUpdate, "source-subscribed-min", 1e-5};
const base::FeatureParam<double> kHnsNewsSourceSubscribedBoost{
    &kHnsNewsFeedUpdate, "source-subscribed-boost", 1};
const base::FeatureParam<double> kHnsNewsChannelSubscribedBoost{
    &kHnsNewsFeedUpdate, "channel-subscribed-boost", 0.2};

const base::FeatureParam<double> kHnsNewsSourceVisitsMin{
    &kHnsNewsFeedUpdate, "source-visits-min", 0.2};

}  // namespace hns_news::features
