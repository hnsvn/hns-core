/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/themes/hns_dark_mode_utils.h"

#include <utility>

#include "base/check_is_test.h"
#include "base/command_line.h"
#include "base/strings/string_util.h"
#include "hns/browser/themes/hns_dark_mode_utils_internal.h"
#include "hns/components/constants/hns_switches.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/channel_info.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/version_info/channel.h"
#include "ui/native_theme/native_theme.h"

namespace {

bool g_is_test_ = false;
bool g_system_dark_mode_enabled_in_test_ = false;

void ClearHnsDarkModeProfilePrefs(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  prefs->ClearPref(kHnsThemeType);
  prefs->ClearPref(kUseOverriddenHnsThemeType);
}

dark_mode::HnsDarkModeType GetDarkModeTypeBasedOnChannel() {
  switch (chrome::GetChannel()) {
    case version_info::Channel::STABLE:
    case version_info::Channel::BETA:
      return dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
    case version_info::Channel::DEV:
    case version_info::Channel::CANARY:
    case version_info::Channel::UNKNOWN:
    default:
      return dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK;
  }
}

dark_mode::HnsDarkModeType GetDarkModeSwitchValue(
    const base::CommandLine& command_line) {
  DCHECK(command_line.HasSwitch(switches::kDarkMode));

  std::string requested_dark_mode_value =
      command_line.GetSwitchValueASCII(switches::kDarkMode);
  std::string requested_dark_mode_value_lower =
      base::ToLowerASCII(requested_dark_mode_value);
  if (requested_dark_mode_value_lower == "light")
    return dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
  if (requested_dark_mode_value_lower == "dark")
    return dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK;

  NOTREACHED();
  return dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
}

}  // namespace

namespace dark_mode {

void MigrateHnsDarkModePrefs(Profile* profile) {
  auto* local_state = g_browser_process->local_state();
  // If migration is done, local state doesn't have default value because
  // they were explicitly set by primary prefs' value. After that, we don't
  // need to try migration again and prefs from profiles are already cleared.
  if (local_state->FindPreference(kHnsDarkMode)->IsDefaultValue()) {
    PrefService* prefs = profile->GetPrefs();
    local_state->SetInteger(kHnsDarkMode,
                            prefs->GetInteger(kHnsThemeType));
  }

  // Clear deprecated prefs.
  ClearHnsDarkModeProfilePrefs(profile);
}

void RegisterHnsDarkModeLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterIntegerPref(
      kHnsDarkMode,
      static_cast<int>(HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT));
}

void RegisterHnsDarkModePrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterIntegerPref(
      kHnsThemeType,
      static_cast<int>(HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT));
  registry->RegisterBooleanPref(kUseOverriddenHnsThemeType, false);
}

bool SystemDarkModeEnabled() {
  if (g_is_test_)
    return g_system_dark_mode_enabled_in_test_;

  return ui::NativeTheme::GetInstanceForNativeUi()->SystemDarkModeSupported();
}

void SetUseSystemDarkModeEnabledForTest(bool enabled) {
  g_is_test_ = true;
  g_system_dark_mode_enabled_in_test_ = enabled;
}

std::string GetStringFromHnsDarkModeType(HnsDarkModeType type) {
  switch (type) {
    case HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT:
      return "Light";
    case HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK:
      return "Dark";
    default:
      NOTREACHED();
      return "Default";
  }
}

void SetHnsDarkModeType(const std::string& type) {
  HnsDarkModeType parsed_type =
      HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT;

  if (type == "Light") {
    parsed_type = HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
  } else if (type == "Dark") {
    parsed_type = HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK;
  }
  SetHnsDarkModeType(parsed_type);
}

void SetHnsDarkModeType(HnsDarkModeType type) {
  g_browser_process->local_state()->SetInteger(kHnsDarkMode,
                                               static_cast<int>(type));
}

HnsDarkModeType GetActiveHnsDarkModeType() {
  // allow override via cli flag
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDarkMode))
    return GetDarkModeSwitchValue(command_line);

  if (!g_browser_process || !g_browser_process->local_state()) {
    // In unittest, local_state() could not be initialzed.
    CHECK_IS_TEST();
    return HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
  }

  HnsDarkModeType type = static_cast<HnsDarkModeType>(
      g_browser_process->local_state()->GetInteger(kHnsDarkMode));
  if (type == HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT) {
    if (!SystemDarkModeEnabled())
      return GetDarkModeTypeBasedOnChannel();

    return ui::NativeTheme::GetInstanceForNativeUi()->ShouldUseDarkColors()
               ? HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK
               : HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
  }
  return type;
}

HnsDarkModeType GetHnsDarkModeType() {
  // allow override via cli flag
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDarkMode))
    return GetDarkModeSwitchValue(command_line);

  if (!g_browser_process || !g_browser_process->local_state()) {
    // In unittest, local_state() could not be initialzed.
    CHECK_IS_TEST();
    return HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT;
  }

  HnsDarkModeType type = static_cast<HnsDarkModeType>(
      g_browser_process->local_state()->GetInteger(kHnsDarkMode));
  if (type == HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT) {
    if (!SystemDarkModeEnabled())
      return GetDarkModeTypeBasedOnChannel();
    return type;
  }
  return type;
}

base::Value::List GetHnsDarkModeTypeList() {
  base::Value::List list;

  if (SystemDarkModeEnabled()) {
    base::Value::Dict system_type;
    system_type.Set(
        "value",
        static_cast<int>(HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT));
    system_type.Set("name", hns_l10n::GetLocalizedResourceUTF16String(
                                IDS_HNS_THEME_TYPE_SYSTEM));
    list.Append(std::move(system_type));
  }

  base::Value::Dict dark_type;
  dark_type.Set("value",
                static_cast<int>(HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK));
  dark_type.Set("name", hns_l10n::GetLocalizedResourceUTF16String(
                            IDS_HNS_THEME_TYPE_DARK));
  list.Append(std::move(dark_type));

  base::Value::Dict light_type;
  light_type.Set(
      "value", static_cast<int>(HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT));
  light_type.Set("name", hns_l10n::GetLocalizedResourceUTF16String(
                             IDS_HNS_THEME_TYPE_LIGHT));
  list.Append(std::move(light_type));

  return list;
}

}  // namespace dark_mode
