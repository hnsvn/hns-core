/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_MANAGER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_MANAGER_H_

#define set_view_factory_for_testing                                           \
  AcceptDenyCancel(const std::vector<PermissionRequest*>& accepted_requests,   \
                   const std::vector<PermissionRequest*>& denied_requests,     \
                   const std::vector<PermissionRequest*>& cancelled_requests); \
  bool ShouldGroupRequests(PermissionRequest* a, PermissionRequest* b);        \
  void set_view_factory_for_testing

#include "src/components/permissions/permission_request_manager.h"  // IWYU pragma: export

#undef set_view_factory_for_testing

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_REQUEST_MANAGER_H_
