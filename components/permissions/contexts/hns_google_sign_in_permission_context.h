// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_PERMISSIONS_CONTEXTS_HNS_GOOGLE_SIGN_IN_PERMISSION_CONTEXT_H_
#define HNS_COMPONENTS_PERMISSIONS_CONTEXTS_HNS_GOOGLE_SIGN_IN_PERMISSION_CONTEXT_H_

#include "components/permissions/permission_context_base.h"
#include "content/public/browser/browser_context.h"

namespace permissions {

class HnsGoogleSignInPermissionContext : public PermissionContextBase {
 public:
  // using PermissionContextBase::RequestPermission;
  explicit HnsGoogleSignInPermissionContext(
      content::BrowserContext* browser_context);
  ~HnsGoogleSignInPermissionContext() override;

  HnsGoogleSignInPermissionContext(
      const HnsGoogleSignInPermissionContext&) = delete;
  HnsGoogleSignInPermissionContext& operator=(
      const HnsGoogleSignInPermissionContext&) = delete;

 private:
  bool IsRestrictedToSecureOrigins() const override;
};

}  // namespace permissions

#endif  // HNS_COMPONENTS_PERMISSIONS_CONTEXTS_HNS_GOOGLE_SIGN_IN_PERMISSION_CONTEXT_H_
