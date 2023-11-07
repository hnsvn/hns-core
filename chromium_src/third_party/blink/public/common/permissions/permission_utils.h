/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_PERMISSIONS_PERMISSION_UTILS_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_PERMISSIONS_PERMISSION_UTILS_H_

// clang-format off
#define NUM                         \
  HNS_ADS,                        \
  HNS_COSMETIC_FILTERING,         \
  HNS_TRACKERS,                   \
  HNS_HTTP_UPGRADABLE_RESOURCES,  \
  HNS_FINGERPRINTING_V2,          \
  HNS_SHIELDS,                    \
  HNS_REFERRERS,                  \
  HNS_COOKIES,                    \
  HNS_SPEEDREADER,                \
  HNS_ETHEREUM,                   \
  HNS_SOLANA,                     \
  HNS_GOOGLE_SIGN_IN,             \
  HNS_LOCALHOST_ACCESS,             \
  NUM
// clang-format on

#include "src/third_party/blink/public/common/permissions/permission_utils.h"  // IWYU pragma: export
#undef NUM

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_PERMISSIONS_PERMISSION_UTILS_H_
