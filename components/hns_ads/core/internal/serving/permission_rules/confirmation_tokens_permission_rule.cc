/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/confirmation_tokens_permission_rule.h"

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {

constexpr int kMinimumConfirmationTokenThreshold = 10;

bool DoesRespectCap() {
  if (!UserHasJoinedHnsRewards()) {
    return true;
  }

  return ConfirmationTokenCount() >= kMinimumConfirmationTokenThreshold;
}

}  // namespace

base::expected<void, std::string>
ConfirmationTokensPermissionRule::ShouldAllow() const {
  if (!DoesRespectCap()) {
    return base::unexpected("You do not have enough confirmation tokens");
  }

  return base::ok();
}

}  // namespace hns_ads
