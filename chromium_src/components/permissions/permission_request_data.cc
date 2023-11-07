/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/permissions/permission_request_data.h"
#include "components/permissions/permission_context_base.h"

namespace permissions {

absl::optional<RequestType> ContentSettingsTypeToRequestTypeIfExists_HnsImpl(
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
      return ContentSettingsTypeToRequestTypeIfExists(content_settings_type);
  }
}

}  // namespace permissions

#define PermissionContextBase PermissionContextBase_ChromiumImpl

#define ContentSettingsTypeToRequestTypeIfExists \
  ContentSettingsTypeToRequestTypeIfExists_HnsImpl

#include "src/components/permissions/permission_request_data.cc"

#undef ContentSettingsTypeToRequestTypeIfExists
#undef PermissionContextBase
