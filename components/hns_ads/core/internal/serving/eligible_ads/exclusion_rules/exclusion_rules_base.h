/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULES_BASE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULES_BASE_H_

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_alias.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_interface.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace hns_ads {

class AntiTargetingResource;
class SubdivisionTargeting;
struct CreativeAdInfo;

class ExclusionRulesBase {
 public:
  ExclusionRulesBase(const ExclusionRulesBase&) = delete;
  ExclusionRulesBase& operator=(const ExclusionRulesBase&) = delete;

  ExclusionRulesBase(ExclusionRulesBase&&) noexcept = delete;
  ExclusionRulesBase& operator=(ExclusionRulesBase&&) noexcept = delete;

  virtual ~ExclusionRulesBase();

  virtual bool ShouldExcludeCreativeAd(const CreativeAdInfo& creative_ad);

 protected:
  ExclusionRulesBase(const AdEventList& ad_events,
                     const SubdivisionTargeting& subdivision_targeting,
                     const AntiTargetingResource& anti_targeting_resource,
                     const BrowsingHistoryList& browsing_history);

  std::vector<std::unique_ptr<ExclusionRuleInterface<CreativeAdInfo>>>
      exclusion_rules_;

  std::set<std::string> uuids_;
  bool AddToCacheIfNeeded(
      const CreativeAdInfo& creative_ad,
      const std::unique_ptr<ExclusionRuleInterface<CreativeAdInfo>>&
          exclusion_rule);

 private:
  bool IsCached(const CreativeAdInfo& creative_ad) const;
  void AddToCache(const std::string& uuid);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULES_BASE_H_
