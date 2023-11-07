/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_APPEARANCE_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_APPEARANCE_HANDLER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

class Profile;

class HnsAppearanceHandler : public settings::SettingsPageUIHandler {
 public:
  HnsAppearanceHandler();
  ~HnsAppearanceHandler() override;

  HnsAppearanceHandler(const HnsAppearanceHandler&) = delete;
  HnsAppearanceHandler& operator=(const HnsAppearanceHandler&) = delete;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void OnHnsDarkModeChanged();
  void OnBackgroundPreferenceChanged(const std::string& pref_name);
  void OnPreferenceChanged(const std::string& pref_name);
  void SetHnsThemeType(const base::Value::List& args);
  void GetHnsThemeType(const base::Value::List& args);
  void GetNewTabShowsOptionsList(const base::Value::List& args);
  void ShouldShowNewTabDashboardSettings(const base::Value::List& args);

  raw_ptr<Profile> profile_ = nullptr;
  PrefChangeRegistrar local_state_change_registrar_;
  PrefChangeRegistrar profile_state_change_registrar_;
};

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_APPEARANCE_HANDLER_H_
