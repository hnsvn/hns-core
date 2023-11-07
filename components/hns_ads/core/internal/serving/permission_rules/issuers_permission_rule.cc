/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/issuers_permission_rule.h"

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {

bool DoesRespectCap() {
  if (!UserHasJoinedHnsRewards()) {
    return true;
  }

  return HasIssuers();
}

}  // namespace

base::expected<void, std::string> IssuersPermissionRule::ShouldAllow() const {
  if (!DoesRespectCap()) {
    return base::unexpected("Missing issuers");
  }

  return base::ok();
}

}  // namespace hns_ads
