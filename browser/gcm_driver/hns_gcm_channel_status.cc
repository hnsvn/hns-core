/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/gcm_driver/hns_gcm_channel_status.h"

#include <memory>

#include "hns/components/constants/pref_names.h"
#include "chrome/browser/gcm/gcm_profile_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_driver_desktop.h"
#include "components/gcm_driver/gcm_profile_service.h"
#include "components/prefs/pref_service.h"

namespace gcm {

const char kHnsGCMStatusKey[] = "hns_gcm_channel_status";

HnsGCMChannelStatus::HnsGCMChannelStatus(Profile* profile, bool enabled)
    : profile_(profile), gcm_enabled_(enabled) {}

// static
HnsGCMChannelStatus* HnsGCMChannelStatus::GetForProfile(
    Profile* profile) {
  HnsGCMChannelStatus* status = static_cast<HnsGCMChannelStatus*>(
      profile->GetUserData(kHnsGCMStatusKey));

  if (!status) {
    bool enabled = profile->GetPrefs()->GetBoolean(kHnsGCMChannelStatus);
    // Object cleanup is handled by SupportsUserData
    profile->SetUserData(
        kHnsGCMStatusKey,
        std::make_unique<HnsGCMChannelStatus>(profile, enabled));
    status = static_cast<HnsGCMChannelStatus*>(
        profile->GetUserData(kHnsGCMStatusKey));
  }
  return status;
}

bool HnsGCMChannelStatus::IsGCMEnabled() const {
  return gcm_enabled_;
}

void HnsGCMChannelStatus::UpdateGCMDriverStatus() {
  if (!profile_)
    return;
  gcm::GCMProfileService* gcm_profile_service =
      gcm::GCMProfileServiceFactory::GetForProfile(profile_);
  if (!gcm_profile_service)
    return;
  gcm::GCMDriver* gcm_driver = gcm_profile_service->driver();
  if (!gcm_driver)
    return;
  gcm_driver->SetEnabled(IsGCMEnabled());
}

}  // namespace gcm
