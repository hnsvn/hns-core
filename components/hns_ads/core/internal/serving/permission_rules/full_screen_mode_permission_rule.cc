/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/full_screen_mode_permission_rule.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/platform/platform_helper.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_feature.h"

namespace hns_ads {

namespace {

bool DoesRespectCap() {
  if (!kShouldOnlyServeAdsInWindowedMode.Get()) {
    return true;
  }

  if (PlatformHelper::GetInstance().IsMobile()) {
    return true;
  }

  return !AdsClientHelper::GetInstance()->IsBrowserInFullScreenMode();
}

}  // namespace

base::expected<void, std::string> FullScreenModePermissionRule::ShouldAllow()
    const {
  if (!DoesRespectCap()) {
    return base::unexpected("Full screen mode");
  }

  return base::ok();
}

}  // namespace hns_ads
