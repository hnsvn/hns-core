/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_GCM_DRIVER_HNS_GCM_CHANNEL_STATUS_H_
#define HNS_BROWSER_GCM_DRIVER_HNS_GCM_CHANNEL_STATUS_H_

#include "base/memory/raw_ptr.h"
#include "base/supports_user_data.h"

class Profile;

namespace gcm {

class HnsGCMChannelStatus : public base::SupportsUserData::Data {
 public:
  explicit HnsGCMChannelStatus(Profile* profile, bool enabled);
  static HnsGCMChannelStatus* GetForProfile(Profile *profile);

  bool IsGCMEnabled() const;
  void UpdateGCMDriverStatus();

 private:
  raw_ptr<Profile> profile_ = nullptr;
  bool gcm_enabled_;
};

}  // namespace gcm

#endif  // HNS_BROWSER_GCM_DRIVER_HNS_GCM_CHANNEL_STATUS_H_
