/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/browser_is_active_permission_rule.h"

#include "hns/components/hns_ads/core/internal/browser/browser_manager.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_feature.h"

namespace hns_ads {

namespace {

bool DoesRespectCap() {
  if (!kShouldOnlyServeAdsIfBrowserIsActive.Get()) {
    return true;
  }

  return BrowserManager::GetInstance().IsBrowserActive() &&
         BrowserManager::GetInstance().IsBrowserInForeground();
}

}  // namespace

base::expected<void, std::string> BrowserIsActivePermissionRule::ShouldAllow()
    const {
  if (!DoesRespectCap()) {
    return base::unexpected("Browser window is not active");
  }

  return base::ok();
}

}  // namespace hns_ads
