/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/platform/platform_helper_win.h"

namespace hns_ads {

namespace {
constexpr char kPlatformName[] = "windows";
}  // namespace

PlatformHelperWin::PlatformHelperWin() = default;

bool PlatformHelperWin::IsMobile() const {
  return false;
}

std::string PlatformHelperWin::GetName() const {
  return kPlatformName;
}

PlatformType PlatformHelperWin::GetType() const {
  return PlatformType::kWindows;
}

}  // namespace hns_ads
