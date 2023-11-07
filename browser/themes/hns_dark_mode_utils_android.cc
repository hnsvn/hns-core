/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/themes/hns_dark_mode_utils.h"

namespace dark_mode {

void MigrateHnsDarkModePrefs(Profile* profile) {
}

void RegisterHnsDarkModeLocalStatePrefs(PrefRegistrySimple* registry) {
}

void RegisterHnsDarkModePrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry) {
}

bool SystemDarkModeEnabled() {
  return false;
}

void SetUseSystemDarkModeEnabledForTest(bool enabled) {
}

std::string GetStringFromHnsDarkModeType(HnsDarkModeType type) {
  return "Default";
}

void SetHnsDarkModeType(const std::string& type) {
}

void SetHnsDarkModeType(HnsDarkModeType type) {
}

HnsDarkModeType GetActiveHnsDarkModeType() {
  return HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT;
}

HnsDarkModeType GetHnsDarkModeType() {
  return HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT;
}

base::Value::List GetHnsDarkModeTypeList() {
  return base::Value::List();
}

void SetSystemDarkMode(HnsDarkModeType type) {
}

}  // namespace dark_mode
