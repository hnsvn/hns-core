/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_GEOLOCATION_HNS_GEOLOCATION_PERMISSION_CONTEXT_DELEGATE_H_
#define HNS_BROWSER_GEOLOCATION_HNS_GEOLOCATION_PERMISSION_CONTEXT_DELEGATE_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/geolocation/geolocation_permission_context_delegate.h"

class Profile;

class HnsGeolocationPermissionContextDelegate
    : public GeolocationPermissionContextDelegate {
 public:
  explicit HnsGeolocationPermissionContextDelegate(
      content::BrowserContext* browser_context);
  HnsGeolocationPermissionContextDelegate(
      const HnsGeolocationPermissionContextDelegate&) = delete;
  HnsGeolocationPermissionContextDelegate& operator=(
      const HnsGeolocationPermissionContextDelegate&) = delete;
  ~HnsGeolocationPermissionContextDelegate() override;

  bool DecidePermission(
      const permissions::PermissionRequestID& id,
      const GURL& requesting_origin,
      bool user_gesture,
      permissions::BrowserPermissionCallback* callback,
      permissions::GeolocationPermissionContext* context) override;

 private:
  raw_ptr<Profile> profile_ = nullptr;
};

#endif  // HNS_BROWSER_GEOLOCATION_HNS_GEOLOCATION_PERMISSION_CONTEXT_DELEGATE_H_
