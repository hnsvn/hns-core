/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/permission_util.h"
#include "components/permissions/permission_uma_util.h"
#include "third_party/blink/public/common/permissions/permission_utils.h"

#define PermissionUtil PermissionUtil_ChromiumImpl

#define PERMISSION_UTIL_PERMISSION_TYPE_TO_CONTENT_SETTINGS_TYPE \
  case PermissionType::HNS_ADS:                                \
    return ContentSettingsType::HNS_ADS;                       \
  case PermissionType::HNS_COSMETIC_FILTERING:                 \
    return ContentSettingsType::HNS_COSMETIC_FILTERING;        \
  case PermissionType::HNS_TRACKERS:                           \
    return ContentSettingsType::HNS_TRACKERS;                  \
  case PermissionType::HNS_HTTP_UPGRADABLE_RESOURCES:          \
    return ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES; \
  case PermissionType::HNS_FINGERPRINTING_V2:                  \
    return ContentSettingsType::HNS_FINGERPRINTING_V2;         \
  case PermissionType::HNS_SHIELDS:                            \
    return ContentSettingsType::HNS_SHIELDS;                   \
  case PermissionType::HNS_REFERRERS:                          \
    return ContentSettingsType::HNS_REFERRERS;                 \
  case PermissionType::HNS_COOKIES:                            \
    return ContentSettingsType::HNS_COOKIES;                   \
  case PermissionType::HNS_SPEEDREADER:                        \
    return ContentSettingsType::HNS_SPEEDREADER;               \
  case PermissionType::HNS_ETHEREUM:                           \
    return ContentSettingsType::HNS_ETHEREUM;                  \
  case PermissionType::HNS_SOLANA:                             \
    return ContentSettingsType::HNS_SOLANA;                    \
  case PermissionType::HNS_GOOGLE_SIGN_IN:                     \
    return ContentSettingsType::HNS_GOOGLE_SIGN_IN;            \
  case PermissionType::HNS_LOCALHOST_ACCESS:                   \
    return ContentSettingsType::HNS_LOCALHOST_ACCESS;

#include "src/components/permissions/permission_util.cc"
#undef PermissionUtil
#undef PERMISSION_UTIL_PERMISSION_TYPE_TO_CONTENT_SETTINGS_TYPE

namespace permissions {

// static
std::string PermissionUtil::GetPermissionString(
    ContentSettingsType content_type) {
  switch (content_type) {
    case ContentSettingsType::HNS_ETHEREUM:
      return "HnsEthereum";
    case ContentSettingsType::HNS_SOLANA:
      return "HnsSolana";
    case ContentSettingsType::HNS_GOOGLE_SIGN_IN:
      return "HnsGoogleSignInPermission";
    case ContentSettingsType::HNS_LOCALHOST_ACCESS:
      return "HnsLocalhostAccessPermission";
    default:
      return PermissionUtil_ChromiumImpl::GetPermissionString(content_type);
  }
}

// static
bool PermissionUtil::GetPermissionType(ContentSettingsType type,
                                       blink::PermissionType* out) {
  if (type == ContentSettingsType::HNS_ETHEREUM ||
      type == ContentSettingsType::HNS_SOLANA) {
    *out = PermissionType::WINDOW_MANAGEMENT;
    return true;
  }
  if (type == ContentSettingsType::HNS_GOOGLE_SIGN_IN) {
    *out = PermissionType::HNS_GOOGLE_SIGN_IN;
    return true;
  }
  if (type == ContentSettingsType::HNS_LOCALHOST_ACCESS) {
    *out = PermissionType::HNS_LOCALHOST_ACCESS;
    return true;
  }

  return PermissionUtil_ChromiumImpl::GetPermissionType(type, out);
}

// static
bool PermissionUtil::IsPermission(ContentSettingsType type) {
  switch (type) {
    case ContentSettingsType::HNS_ETHEREUM:
    case ContentSettingsType::HNS_SOLANA:
    case ContentSettingsType::HNS_GOOGLE_SIGN_IN:
    case ContentSettingsType::HNS_LOCALHOST_ACCESS:
      return true;
    default:
      return PermissionUtil_ChromiumImpl::IsPermission(type);
  }
}

PermissionType PermissionUtil::ContentSettingTypeToPermissionType(
    ContentSettingsType permission) {
  switch (permission) {
    case ContentSettingsType::HNS_ADS:
      return PermissionType::HNS_ADS;
    case ContentSettingsType::HNS_COSMETIC_FILTERING:
      return PermissionType::HNS_COSMETIC_FILTERING;
    case ContentSettingsType::HNS_TRACKERS:
      return PermissionType::HNS_TRACKERS;
    case ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES:
      return PermissionType::HNS_HTTP_UPGRADABLE_RESOURCES;
    case ContentSettingsType::HNS_FINGERPRINTING_V2:
      return PermissionType::HNS_FINGERPRINTING_V2;
    case ContentSettingsType::HNS_SHIELDS:
      return PermissionType::HNS_SHIELDS;
    case ContentSettingsType::HNS_REFERRERS:
      return PermissionType::HNS_REFERRERS;
    case ContentSettingsType::HNS_COOKIES:
      return PermissionType::HNS_COOKIES;
    case ContentSettingsType::HNS_SPEEDREADER:
      return PermissionType::HNS_SPEEDREADER;
    case ContentSettingsType::HNS_ETHEREUM:
      return PermissionType::HNS_ETHEREUM;
    case ContentSettingsType::HNS_SOLANA:
      return PermissionType::HNS_SOLANA;
    case ContentSettingsType::HNS_GOOGLE_SIGN_IN:
      return PermissionType::HNS_GOOGLE_SIGN_IN;
    case ContentSettingsType::HNS_LOCALHOST_ACCESS:
      return PermissionType::HNS_LOCALHOST_ACCESS;
    default:
      return PermissionUtil_ChromiumImpl::ContentSettingTypeToPermissionType(
          permission);
  }
}

GURL PermissionUtil::GetCanonicalOrigin(ContentSettingsType permission,
                                        const GURL& requesting_origin,
                                        const GURL& embedding_origin) {
  // Use requesting_origin which will have ethereum or solana address info.
  if (permission == ContentSettingsType::HNS_ETHEREUM ||
      permission == ContentSettingsType::HNS_SOLANA)
    return requesting_origin;

  return PermissionUtil_ChromiumImpl::GetCanonicalOrigin(
      permission, requesting_origin, embedding_origin);
}

}  // namespace permissions
