/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/inline_content_ads/inline_content_ads_per_day_permission_rule.h"

#include <vector>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/ads/inline_content_ad/inline_content_ad_feature.h"
#include "hns/components/hns_ads/core/internal/common/time/time_constraint_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_events.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

namespace {

constexpr base::TimeDelta kTimeConstraint = base::Days(1);

bool DoesRespectCap(const std::vector<base::Time>& history) {
  return DoesHistoryRespectRollingTimeConstraint(
      history, kTimeConstraint,
      /*cap*/ kMaximumInlineContentAdsPerDay.Get());
}

}  // namespace

base::expected<void, std::string>
InlineContentAdsPerDayPermissionRule::ShouldAllow() const {
  const std::vector<base::Time> history =
      GetAdEventHistory(AdType::kInlineContentAd, ConfirmationType::kServed);
  if (!DoesRespectCap(history)) {
    return base::unexpected(
        "You have exceeded the allowed inline content ads per day");
  }

  return base::ok();
}

}  // namespace hns_ads
