/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/search_result_ads/search_result_ad_permission_rules.h"

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/search_result_ads/search_result_ads_per_day_permission_rule.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/search_result_ads/search_result_ads_per_hour_permission_rule.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

// static
bool SearchResultAdPermissionRules::HasPermission() {
  if (!UserHasJoinedHnsRewards()) {
    return true;
  }

  if (!PermissionRulesBase::HasPermission()) {
    return false;
  }

  const SearchResultAdsPerDayPermissionRule ads_per_day_permission_rule;
  if (!ShouldAllow(ads_per_day_permission_rule)) {
    return false;
  }

  const SearchResultAdsPerHourPermissionRule ads_per_hour_permission_rule;
  return ShouldAllow(ads_per_hour_permission_rule);
}

}  // namespace hns_ads
