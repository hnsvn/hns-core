/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/permission_util.h"
#include "third_party/blink/public/common/permissions/permission_utils.h"

#define NUM                                             \
  HNS_ADS:                                            \
  case PermissionType::HNS_COSMETIC_FILTERING:        \
  case PermissionType::HNS_TRACKERS:                  \
  case PermissionType::HNS_HTTP_UPGRADABLE_RESOURCES: \
  case PermissionType::HNS_FINGERPRINTING_V2:         \
  case PermissionType::HNS_SHIELDS:                   \
  case PermissionType::HNS_REFERRERS:                 \
  case PermissionType::HNS_COOKIES:                   \
  case PermissionType::HNS_SPEEDREADER:               \
  case PermissionType::HNS_ETHEREUM:                  \
  case PermissionType::HNS_SOLANA:                    \
  case PermissionType::HNS_GOOGLE_SIGN_IN:            \
  case PermissionType::HNS_LOCALHOST_ACCESS:          \
  case PermissionType::NUM

#include "src/android_webview/browser/aw_permission_manager.cc"
#undef NUM
