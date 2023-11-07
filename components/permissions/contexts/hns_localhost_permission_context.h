// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_PERMISSIONS_CONTEXTS_HNS_LOCALHOST_PERMISSION_CONTEXT_H_
#define HNS_COMPONENTS_PERMISSIONS_CONTEXTS_HNS_LOCALHOST_PERMISSION_CONTEXT_H_

#include "components/permissions/permission_context_base.h"
#include "content/public/browser/browser_context.h"

namespace permissions {

class HnsLocalhostPermissionContext : public PermissionContextBase {
 public:
  // using PermissionContextBase::RequestPermission;
  explicit HnsLocalhostPermissionContext(
      content::BrowserContext* browser_context);
  ~HnsLocalhostPermissionContext() override;

  HnsLocalhostPermissionContext(const HnsLocalhostPermissionContext&) =
      delete;
  HnsLocalhostPermissionContext& operator=(
      const HnsLocalhostPermissionContext&) = delete;

 private:
  bool IsRestrictedToSecureOrigins() const override;
};

}  // namespace permissions

#endif  // HNS_COMPONENTS_PERMISSIONS_CONTEXTS_HNS_LOCALHOST_PERMISSION_CONTEXT_H_
