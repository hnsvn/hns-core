/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/gcm_driver/hns_gcm_utils.h"

#include "base/values.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/prefs/pref_service.h"

namespace gcm {

// Chromium pref deprecated as of 01/2020.
const char kGCMChannelStatus[] = "gcm.channel_status";

void RegisterGCMProfilePrefs(user_prefs::PrefRegistrySyncable* registry) {
  // Deprecated Chromium pref.
  registry->RegisterBooleanPref(kGCMChannelStatus, false);
  // Current Hns equivalent of the deprecated pref.
  registry->RegisterBooleanPref(kHnsGCMChannelStatus, false);
}

void MigrateGCMPrefs(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  // The default was false (see above).
  auto* pref = prefs->FindPreference(kGCMChannelStatus);
  if (pref && !pref->IsDefaultValue())
    prefs->SetBoolean(kHnsGCMChannelStatus, pref->GetValue()->GetBool());
  prefs->ClearPref(kGCMChannelStatus);
}

}  // namespace gcm
