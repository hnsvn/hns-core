/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/permission_uma_util.h"

#include "components/permissions/permissions_client.h"

// Since we don't do UMA just reuse an existing UMA type instead of adding one.
#define HNS_GET_UMA_VALUE_FOR_REQUEST_TYPE         \
  case RequestType::kWidevine:                       \
  case RequestType::kHnsEthereum:                  \
  case RequestType::kHnsSolana:                    \
  case RequestType::kHnsGoogleSignInPermission:    \
  case RequestType::kHnsLocalhostAccessPermission: \
    return RequestTypeForUma::PERMISSION_VR;

// We do not record permissions UKM and this can save us from patching
// in RecordPermissionAction for unhandling switch cases for Hns's content
// settings type.
#define GetUkmSourceId             \
  GetSettingsMap(browser_context); \
  if (true)                        \
    return;                        \
  PermissionsClient::Get()->GetUkmSourceId

#include "src/components/permissions/permission_uma_util.cc"
#undef HNS_GET_UMA_VALUE_FOR_REQUEST_TYPE
#undef GetUkmSourceId
