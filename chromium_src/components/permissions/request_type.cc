/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/request_type.h"

#include "build/build_config.h"

#if BUILDFLAG(IS_ANDROID)
#include "components/resources/android/theme_resources.h"
#else
#include "components/vector_icons/vector_icons.h"
#include "ui/gfx/vector_icon_types.h"
#endif

#if BUILDFLAG(IS_ANDROID)
namespace {
constexpr auto kAndroidInfobarPermissionCookie =
    IDR_ANDROID_INFOBAR_PERMISSION_COOKIE;
}  // namespace
#else
namespace vector_icons {
constexpr auto& kMicIconValue = vector_icons::kMicIcon;
}  // namespace vector_icons
#endif

// Add Hns cases into GetIconIdAndroid.
// kWidevine is not expected to happen here as Widevine is not enabled in
// Android, we add this case here just to avoid build error due to unhandled
// cases in the switch.
//
// TODO(jocelyn): Might need to update icon when we have ethereum.enable UI
// support in Android.
#define IDR_ANDROID_INFOBAR_PERMISSION_COOKIE        \
  kAndroidInfobarPermissionCookie;                   \
  case RequestType::kWidevine:                       \
  case RequestType::kHnsEthereum:                  \
  case RequestType::kHnsSolana:                    \
  case RequestType::kHnsGoogleSignInPermission:    \
  case RequestType::kHnsLocalhostAccessPermission: \
    return IDR_ANDROID_INFOBAR_PERMISSION_COOKIE

// Add Hns cases into GetIconIdDesktop.
#define kMicIcon                                     \
  kMicIconValue;                                     \
  case RequestType::kWidevine:                       \
  case RequestType::kHnsEthereum:                  \
  case RequestType::kHnsSolana:                    \
  case RequestType::kHnsGoogleSignInPermission:    \
  case RequestType::kHnsLocalhostAccessPermission: \
    return vector_icons::kExtensionIcon

#define HNS_PERMISSION_KEY_FOR_REQUEST_TYPE                     \
  case permissions::RequestType::kWidevine:                       \
    return "widevine";                                            \
  case permissions::RequestType::kHnsEthereum:                  \
    return "hns_ethereum";                                      \
  case permissions::RequestType::kHnsSolana:                    \
    return "hns_solana";                                        \
  case permissions::RequestType::kHnsGoogleSignInPermission:    \
    return "hns_google_sign_in";                                \
  case permissions::RequestType::kHnsLocalhostAccessPermission: \
    return "hns_localhost_access";

#define ContentSettingsTypeToRequestType \
  ContentSettingsTypeToRequestType_ChromiumImpl

#define RequestTypeToContentSettingsType \
  RequestTypeToContentSettingsType_ChromiumImpl

#define IsRequestablePermissionType IsRequestablePermissionType_ChromiumImpl

#include "src/components/permissions/request_type.cc"

#undef HNS_PERMISSION_KEY_FOR_REQUEST_TYPE
#undef IDR_ANDROID_INFOBAR_PERMISSION_COOKIE
#undef kMicIcon
#undef ContentSettingsTypeToRequestType
#undef RequestTypeToContentSettingsType
#undef IsRequestablePermissionType

namespace permissions {

RequestType ContentSettingsTypeToRequestType(
    ContentSettingsType content_settings_type) {
  switch (content_settings_type) {
    case ContentSettingsType::HNS_ETHEREUM:
      return RequestType::kHnsEthereum;
    case ContentSettingsType::HNS_SOLANA:
      return RequestType::kHnsSolana;
    case ContentSettingsType::HNS_GOOGLE_SIGN_IN:
      return RequestType::kHnsGoogleSignInPermission;
    case ContentSettingsType::HNS_LOCALHOST_ACCESS:
      return RequestType::kHnsLocalhostAccessPermission;
    default:
      return ContentSettingsTypeToRequestType_ChromiumImpl(
          content_settings_type);
  }
}

absl::optional<ContentSettingsType> RequestTypeToContentSettingsType(
    RequestType request_type) {
  switch (request_type) {
    case RequestType::kHnsGoogleSignInPermission:
      return ContentSettingsType::HNS_GOOGLE_SIGN_IN;
    case RequestType::kHnsLocalhostAccessPermission:
      return ContentSettingsType::HNS_LOCALHOST_ACCESS;
    case RequestType::kHnsEthereum:
      return ContentSettingsType::HNS_ETHEREUM;
    case RequestType::kHnsSolana:
      return ContentSettingsType::HNS_SOLANA;
    default:
      return RequestTypeToContentSettingsType_ChromiumImpl(request_type);
  }
}

bool IsRequestablePermissionType(ContentSettingsType content_settings_type) {
  switch (content_settings_type) {
    case ContentSettingsType::HNS_GOOGLE_SIGN_IN:
    case ContentSettingsType::HNS_LOCALHOST_ACCESS:
    case ContentSettingsType::HNS_ETHEREUM:
    case ContentSettingsType::HNS_SOLANA:
      return true;
    default:
      return IsRequestablePermissionType_ChromiumImpl(content_settings_type);
  }
}

}  // namespace permissions
