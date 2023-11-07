/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/platform_user_data.h"

#include <string>

#include "hns/components/hns_ads/core/internal/common/platform/platform_helper.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {
constexpr char kPlatformKey[] = "platform";
}  // namespace

base::Value::Dict BuildPlatformUserData() {
  base::Value::Dict user_data;

  if (!UserHasJoinedHnsRewards()) {
    return user_data;
  }

  const std::string platform_name = PlatformHelper::GetInstance().GetName();
  if (!platform_name.empty()) {
    user_data.Set(kPlatformKey, platform_name);
  }

  return user_data;
}

}  // namespace hns_ads
