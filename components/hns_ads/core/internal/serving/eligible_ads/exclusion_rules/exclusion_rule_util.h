/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULE_UTIL_H_

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_interface.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

class ConfirmationType;
struct CreativeAdInfo;

bool DoesRespectCampaignCap(const CreativeAdInfo& creative_ad,
                            const AdEventList& ad_events,
                            const ConfirmationType& confirmation_type,
                            base::TimeDelta time_constraint,
                            size_t cap);
bool DoesRespectCreativeSetCap(const CreativeAdInfo& creative_ad,
                               const AdEventList& ad_events,
                               const ConfirmationType& confirmation_type,
                               base::TimeDelta time_constraint,
                               size_t cap);
bool DoesRespectCreativeCap(const CreativeAdInfo& creative_ad,
                            const AdEventList& ad_events,
                            const ConfirmationType& confirmation_type,
                            base::TimeDelta time_constraint,
                            size_t cap);

template <typename T>
bool ShouldInclude(const T& ad,
                   const ExclusionRuleInterface<T>* const exclusion_rule) {
  CHECK(exclusion_rule);

  const auto result = exclusion_rule->ShouldInclude(ad);
  if (!result.has_value()) {
    BLOG(2, result.error());
    return false;
  }

  return true;
}

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_EXCLUSION_RULES_EXCLUSION_RULE_UTIL_H_
