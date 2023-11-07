/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/build_channel_user_data.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads {

namespace {
constexpr char kBuildChannelKey[] = "buildChannel";
}  // namespace

base::Value::Dict BuildBuildChannelUserData() {
  base::Value::Dict user_data;

  if (!UserHasJoinedHnsRewards()) {
    return user_data;
  }

  const auto& build_channel = GlobalState::GetInstance()->BuildChannel();
  CHECK(!build_channel.name.empty());

  user_data.Set(kBuildChannelKey, build_channel.name);

  return user_data;
}

}  // namespace hns_ads
