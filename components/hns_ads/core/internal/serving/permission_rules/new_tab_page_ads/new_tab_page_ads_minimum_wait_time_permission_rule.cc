/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/new_tab_page_ads/new_tab_page_ads_minimum_wait_time_permission_rule.h"

#include <vector>

#include "base/time/time.h"
#include "base/types/expected.h"
#include "hns/components/hns_ads/core/internal/ads/new_tab_page_ad/new_tab_page_ad_feature.h"
#include "hns/components/hns_ads/core/internal/common/time/time_constraint_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_events.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

namespace {

constexpr int kMinimumWaitTimeCap = 1;

bool DoesRespectCap(const std::vector<base::Time>& history) {
  return DoesHistoryRespectRollingTimeConstraint(
      history, /*time_constraint*/ kNewTabPageAdMinimumWaitTime.Get(),
      kMinimumWaitTimeCap);
}

}  // namespace

base::expected<void, std::string>
NewTabPageAdMinimumWaitTimePermissionRule::ShouldAllow() const {
  const std::vector<base::Time> history =
      GetAdEventHistory(AdType::kNewTabPageAd, ConfirmationType::kServed);
  if (!DoesRespectCap(history)) {
    return base::unexpected(
        "New tab page ad cannot be shown as minimum wait time has not passed");
  }

  return base::ok();
}

}  // namespace hns_ads
