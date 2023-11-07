/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_TOTAL_MAX_EXCLUSION_RULE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_TOTAL_MAX_EXCLUSION_RULE_H_

#include <string>

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_interface.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace hns_ads {

struct CreativeAdInfo;

class TotalMaxExclusionRule final
    : public ExclusionRuleInterface<CreativeAdInfo> {
 public:
  explicit TotalMaxExclusionRule(AdEventList ad_events);

  TotalMaxExclusionRule(const TotalMaxExclusionRule&) = delete;
  TotalMaxExclusionRule& operator=(const TotalMaxExclusionRule&) = delete;

  TotalMaxExclusionRule(TotalMaxExclusionRule&&) noexcept = delete;
  TotalMaxExclusionRule& operator=(TotalMaxExclusionRule&&) noexcept = delete;

  ~TotalMaxExclusionRule() override;

  std::string GetUuid(const CreativeAdInfo& creative_ad) const override;

  base::expected<void, std::string> ShouldInclude(
      const CreativeAdInfo& creative_ad) const override;

 private:
  AdEventList ad_events_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_TOTAL_MAX_EXCLUSION_RULE_H_
