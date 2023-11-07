/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_DATA_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_DATA_H_

#ifndef PermissionContextBase
#define HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_DATA_PERMISSION_CONTEXT_BASE
#define PermissionContextBase PermissionContextBase_ChromiumImpl
#endif

#include "src/components/permissions/permission_request_data.h"  // IWYU pragma: export

#ifdef HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_DATA_PERMISSION_CONTEXT_BASE
#undef PermissionContextBase
#endif

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_DATA_H_
