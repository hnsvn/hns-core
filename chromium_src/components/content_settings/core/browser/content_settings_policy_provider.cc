/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "hns/components/constants/pref_names.h"

#define HNS_MANAGED_CONTENT_SETTINGS                                         \
  {kManagedHnsShieldsDisabledForUrls, ContentSettingsType::HNS_SHIELDS,    \
   CONTENT_SETTING_BLOCK},                                                     \
      {kManagedHnsShieldsEnabledForUrls, ContentSettingsType::HNS_SHIELDS, \
       CONTENT_SETTING_ALLOW},

#define HNS_MANAGED_PREFS \
  kManagedHnsShieldsDisabledForUrls, kManagedHnsShieldsEnabledForUrls,

#include "src/components/content_settings/core/browser/content_settings_policy_provider.cc"
#undef HNS_MANAGED_PREFS
#undef HNS_MANAGED_CONTENT_SETTINGS
