/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/network_connection_permission_rule.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_feature.h"

namespace hns_ads {

namespace {

bool DoesRespectCap() {
  if (!kShouldOnlyServeAdsWithValidInternetConnection.Get()) {
    return true;
  }

  return AdsClientHelper::GetInstance()->IsNetworkConnectionAvailable();
}

}  // namespace

base::expected<void, std::string> NetworkConnectionPermissionRule::ShouldAllow()
    const {
  if (!DoesRespectCap()) {
    return base::unexpected("Network connection is unavailable");
  }

  return base::ok();
}

}  // namespace hns_ads
