/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_GCM_DRIVER_GCM_DRIVER_DESKTOP_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_GCM_DRIVER_GCM_DRIVER_DESKTOP_H_

#include "src/components/gcm_driver/gcm_driver_desktop.h"  // IWYU pragma: export

namespace gcm {

class HnsGCMDriverDesktop : public GCMDriverDesktop {
 public:
  using GCMDriverDesktop::GCMDriverDesktop;
  ~HnsGCMDriverDesktop() override;

  void SetEnabled(bool enabled) override;

 protected:
  // GCMDriver implementation:
  GCMClient::Result EnsureStarted(GCMClient::StartMode start_mode) override;

 private:
  bool enabled_;
};

}  // namespace gcm

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_GCM_DRIVER_GCM_DRIVER_DESKTOP_H_
