/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/notification_ads/notification_ad_embedding_exclusion_rule.h"

#include "base/strings/string_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/serving/notification_ad_serving_feature.h"

namespace hns_ads {

namespace {

constexpr int kCompatibleServingVersion = 3;

bool DoesRespectCap(const CreativeAdInfo& creative_ad) {
  if (kNotificationAdServingVersion.Get() != kCompatibleServingVersion) {
    return true;
  }

  return !creative_ad.embedding.empty();
}

}  // namespace

std::string EmbeddingExclusionRule::GetUuid(
    const CreativeAdInfo& creative_ad) const {
  return creative_ad.creative_set_id;
}

base::expected<void, std::string> EmbeddingExclusionRule::ShouldInclude(
    const CreativeAdInfo& creative_ad) const {
  if (!DoesRespectCap(creative_ad)) {
    return base::unexpected(base::ReplaceStringPlaceholders(
        "creativeSetId $1 does not have a matching embedding",
        {creative_ad.creative_set_id}, nullptr));
  }

  return base::ok();
}

}  // namespace hns_ads
