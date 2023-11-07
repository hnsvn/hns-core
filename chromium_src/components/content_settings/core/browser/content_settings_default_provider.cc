/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"

#if !BUILDFLAG(IS_IOS)
#define HNS_DISCARD_OR_MIGRATE_OBSOLETE_PREFERENCES           \
  const std::string& autoplay_pref =                            \
      GetPrefName(ContentSettingsType::AUTOPLAY);               \
  if (IntToContentSetting(prefs_->GetInteger(autoplay_pref)) == \
      ContentSetting::CONTENT_SETTING_ASK) {                    \
    prefs_->ClearPref(autoplay_pref);                           \
  }
#else
#define HNS_DISCARD_OR_MIGRATE_OBSOLETE_PREFERENCES
#endif

#include "src/components/content_settings/core/browser/content_settings_default_provider.cc"

#undef HNS_DISCARD_OR_MIGRATE_OBSOLETE_PREFERENCES
