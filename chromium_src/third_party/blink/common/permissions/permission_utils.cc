/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/public/mojom/permissions_policy/permissions_policy_feature.mojom.h"

#define PERMISSION_UTIL_GET_PERMISSION_STRING           \
  case PermissionType::HNS_ADS:                       \
    return "HnsAds";                                  \
  case PermissionType::HNS_COSMETIC_FILTERING:        \
    return "HnsCosmeticFiltering";                    \
  case PermissionType::HNS_TRACKERS:                  \
    return "HnsTrackers";                             \
  case PermissionType::HNS_HTTP_UPGRADABLE_RESOURCES: \
    return "HnsHttpUpgradableResource";               \
  case PermissionType::HNS_FINGERPRINTING_V2:         \
    return "HnsFingerprintingV2";                     \
  case PermissionType::HNS_SHIELDS:                   \
    return "HnsShields";                              \
  case PermissionType::HNS_REFERRERS:                 \
    return "HnsReferrers";                            \
  case PermissionType::HNS_COOKIES:                   \
    return "HnsCookies";                              \
  case PermissionType::HNS_SPEEDREADER:               \
    return "HnsSpeedreaders";                         \
  case PermissionType::HNS_GOOGLE_SIGN_IN:            \
    return "HnsGoogleSignInPermission";               \
  case PermissionType::HNS_LOCALHOST_ACCESS:          \
    return "HnsLocalhostAccessPermission";            \
  case PermissionType::HNS_ETHEREUM:                  \
    return "HnsEthereum";                             \
  case PermissionType::HNS_SOLANA:                    \
    return "HnsSolana";

#define kDisplayCapture                                 \
  kDisplayCapture;                                      \
  case PermissionType::HNS_ETHEREUM:                  \
    return mojom::PermissionsPolicyFeature::kEthereum;  \
  case PermissionType::HNS_SOLANA:                    \
    return mojom::PermissionsPolicyFeature::kSolana;    \
  case PermissionType::HNS_ADS:                       \
  case PermissionType::HNS_COSMETIC_FILTERING:        \
  case PermissionType::HNS_TRACKERS:                  \
  case PermissionType::HNS_HTTP_UPGRADABLE_RESOURCES: \
  case PermissionType::HNS_FINGERPRINTING_V2:         \
  case PermissionType::HNS_SHIELDS:                   \
  case PermissionType::HNS_REFERRERS:                 \
  case PermissionType::HNS_COOKIES:                   \
  case PermissionType::HNS_SPEEDREADER:               \
  case PermissionType::HNS_GOOGLE_SIGN_IN:            \
  case PermissionType::HNS_LOCALHOST_ACCESS:          \
    return absl::nullopt

#include "src/third_party/blink/common/permissions/permission_utils.cc"

#undef kDisplayCapture
#undef PERMISSION_UTIL_GET_PERMISSION_STRING
