/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_THEMES_HNS_DARK_MODE_UTILS_H_
#define HNS_BROWSER_THEMES_HNS_DARK_MODE_UTILS_H_

#include <string>

#include "base/values.h"
#include "build/build_config.h"

class PrefRegistrySimple;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace dark_mode {

enum class HnsDarkModeType {
  // DEFAULT type acts as two ways depends on system theme mode.
  // If system dark mode is not supported, we override it with channel based
  // policy. In this case, user can see dark or light option in settings.
  // Otherwise, it acts like system dark mode mode. It respects system's dark
  // mode. In this case, user can see all three options in theme settings.
  HNS_DARK_MODE_TYPE_DEFAULT,
  HNS_DARK_MODE_TYPE_DARK,
  HNS_DARK_MODE_TYPE_LIGHT,
};

// APIs for prefs.
void MigrateHnsDarkModePrefs(Profile* profile);
void RegisterHnsDarkModePrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry);
void RegisterHnsDarkModeLocalStatePrefs(PrefRegistrySimple* registry);

std::string GetStringFromHnsDarkModeType(HnsDarkModeType type);
base::Value::List GetHnsDarkModeTypeList();
void SetHnsDarkModeType(const std::string& type);
void SetHnsDarkModeType(HnsDarkModeType type);
// Returns current effective theme type. dark or light.
HnsDarkModeType GetActiveHnsDarkModeType();
// Returns current theme type.
// dark/light will be returned if platform doesn't support system dark mode.
// Otherwise, returns default/dark/light.
HnsDarkModeType GetHnsDarkModeType();
bool SystemDarkModeEnabled();
void SetUseSystemDarkModeEnabledForTest(bool enabled);

// When system supports system per-application system theme changing, set it.
// Currently, only MacOS support it.
// Otherewise, we need to overrides from native theme level and explicitly
// notifying to let observers know.
// By overriding, base ui components also use same hns theme type.
void SetSystemDarkMode(HnsDarkModeType type);

}  // namespace dark_mode

#endif  // HNS_BROWSER_THEMES_HNS_DARK_MODE_UTILS_H_
