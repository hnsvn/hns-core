/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/test/base/hns_testing_profile.h"

#include "hns/components/constants/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"

HnsTestingProfile::HnsTestingProfile(const base::FilePath& path,
                                         Delegate* delegate)
    : TestingProfile(path, delegate) {}

HnsTestingProfile::HnsTestingProfile() : TestingProfile() {
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  GetPrefs()->SetBoolean(kHnsGCMChannelStatus, true);
#endif
}
