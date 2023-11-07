/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_

#define kStorageAccess                                     \
  kStorageAccess, kWidevine, kHnsEthereum, kHnsSolana, \
      kHnsGoogleSignInPermission, kHnsLocalhostAccessPermission

#define ContentSettingsTypeToRequestType \
  ContentSettingsTypeToRequestType_ChromiumImpl

#define RequestTypeToContentSettingsType \
  RequestTypeToContentSettingsType_ChromiumImpl

#define IsRequestablePermissionType IsRequestablePermissionType_ChromiumImpl

#include "src/components/permissions/request_type.h"  // IWYU pragma: export

#undef kStorageAccess
#undef ContentSettingsTypeToRequestType
#undef RequestTypeToContentSettingsType
#undef IsRequestablePermissionType

namespace permissions {

RequestType ContentSettingsTypeToRequestType(
    ContentSettingsType content_settings_type);

absl::optional<ContentSettingsType> RequestTypeToContentSettingsType(
    RequestType request_type);

bool IsRequestablePermissionType(ContentSettingsType content_settings_type);

}  // namespace permissions

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_
