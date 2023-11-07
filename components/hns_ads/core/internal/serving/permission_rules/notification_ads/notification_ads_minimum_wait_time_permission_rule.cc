/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/notification_ads/notification_ads_minimum_wait_time_permission_rule.h"

#include <vector>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/platform/platform_helper.h"
#include "hns/components/hns_ads/core/internal/common/time/time_constraint_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_events.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

namespace {

constexpr int kMinimumWaitTimeCap = 1;

bool DoesRespectCap(const std::vector<base::Time>& history) {
  return DoesHistoryRespectRollingTimeConstraint(
      history,
      /*time_constraint*/ base::Hours(1) / GetMaximumNotificationAdsPerHour(),
      kMinimumWaitTimeCap);
}

}  // namespace

base::expected<void, std::string>
NotificationAdMinimumWaitTimePermissionRule::ShouldAllow() const {
  if (PlatformHelper::GetInstance().IsMobile()) {
    // Ads are periodically served on mobile so they will never be served before
    // the minimum wait time has passed
    return base::ok();
  }

  const std::vector<base::Time> history =
      GetAdEventHistory(AdType::kNotificationAd, ConfirmationType::kServed);
  if (!DoesRespectCap(history)) {
    return base::unexpected(
        "Notification ad cannot be shown as minimum wait time has not passed");
  }

  return base::ok();
}

}  // namespace hns_ads
