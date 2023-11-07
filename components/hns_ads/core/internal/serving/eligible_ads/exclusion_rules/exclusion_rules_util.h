/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULES_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULES_UTIL_H_

#include <iterator>

#include "base/check.h"
#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rules_base.h"
#include "hns/components/hns_ads/core/public/ad_info.h"

namespace hns_ads {

template <typename T>
bool ShouldCapLastServedCreativeAd(const T& creative_ads) {
  return creative_ads.size() != 1;
}

template <typename T>
T ApplyExclusionRules(const T& creative_ads,
                      const AdInfo& last_served_ad,
                      ExclusionRulesBase* exclusion_rules) {
  CHECK(exclusion_rules);

  const bool should_cap_last_served_creative_ad =
      ShouldCapLastServedCreativeAd(creative_ads);

  T filtered_creative_ads;

  base::ranges::copy_if(
      creative_ads, std::back_inserter(filtered_creative_ads),
      [exclusion_rules, &last_served_ad,
       &should_cap_last_served_creative_ad](const CreativeAdInfo& creative_ad) {
        const bool should_exclude =
            exclusion_rules->ShouldExcludeCreativeAd(creative_ad) ||
            (should_cap_last_served_creative_ad &&
             creative_ad.creative_instance_id ==
                 last_served_ad.creative_instance_id);

        return !should_exclude;
      });

  return filtered_creative_ads;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULES_UTIL_H_
