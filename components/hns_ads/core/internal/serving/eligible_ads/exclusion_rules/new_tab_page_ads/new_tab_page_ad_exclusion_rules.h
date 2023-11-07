/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EXCLUSION_RULES_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EXCLUSION_RULES_H_

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rules_base.h"

namespace hns_ads {

class AntiTargetingResource;
class SubdivisionTargeting;

class NewTabPageAdExclusionRules final : public ExclusionRulesBase {
 public:
  NewTabPageAdExclusionRules(
      const AdEventList& ad_events,
      const SubdivisionTargeting& subdivision_targeting,
      const AntiTargetingResource& anti_targeting_resource,
      const BrowsingHistoryList& browsing_history);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EXCLUSION_RULES_H_
