/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/notification_ads/notification_ad_exclusion_rules.h"

#include <memory>
#include <utility>

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/creative_instance_exclusion_rule.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/notification_ads/notification_ad_dismissed_exclusion_rule.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/notification_ads/notification_ad_embedding_exclusion_rule.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/anti_targeting/resource/anti_targeting_resource.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"

namespace hns_ads {

NotificationAdExclusionRules::NotificationAdExclusionRules(
    const AdEventList& ad_events,
    const SubdivisionTargeting& subdivision_targeting,
    const AntiTargetingResource& anti_targeting_resource,
    const BrowsingHistoryList& browsing_history)
    : ExclusionRulesBase(ad_events,
                         subdivision_targeting,
                         anti_targeting_resource,
                         browsing_history) {
  auto creative_instance_exclusion_rule =
      std::make_unique<CreativeInstanceExclusionRule>(ad_events);
  exclusion_rules_.push_back(std::move(creative_instance_exclusion_rule));

  auto dismissed_exclusion_rule =
      std::make_unique<NotificationAdDismissedExclusionRule>(ad_events);
  exclusion_rules_.push_back(std::move(dismissed_exclusion_rule));

  auto embedding_exclusion_rule = std::make_unique<EmbeddingExclusionRule>();
  exclusion_rules_.push_back(std::move(embedding_exclusion_rule));
}

NotificationAdExclusionRules::~NotificationAdExclusionRules() = default;

}  // namespace hns_ads
