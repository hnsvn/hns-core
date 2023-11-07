/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_PERMISSIONS_HNS_PERMISSION_MANAGER_H_
#define HNS_COMPONENTS_PERMISSIONS_HNS_PERMISSION_MANAGER_H_

#include <vector>

#include "components/permissions/permission_manager.h"

namespace permissions {

class HnsPermissionManager : public PermissionManager {
 public:
  HnsPermissionManager(content::BrowserContext* browser_context,
                         PermissionContextMap permission_contexts);
  ~HnsPermissionManager() override = default;

  HnsPermissionManager(const HnsPermissionManager&) = delete;
  HnsPermissionManager& operator=(const HnsPermissionManager&) = delete;

  void Shutdown() override;

  void RequestPermissionsForOrigin(
      const std::vector<blink::PermissionType>& permissions,
      content::RenderFrameHost* render_frame_host,
      const GURL& requesting_origin,
      bool user_gesture,
      base::OnceCallback<
          void(const std::vector<blink::mojom::PermissionStatus>&)> callback)
      override;

  blink::mojom::PermissionStatus GetPermissionStatusForOrigin(
      blink::PermissionType permission,
      content::RenderFrameHost* render_frame_host,
      const GURL& requesting_origin) override;
};

}  // namespace permissions

#endif  // HNS_COMPONENTS_PERMISSIONS_HNS_PERMISSION_MANAGER_H_
