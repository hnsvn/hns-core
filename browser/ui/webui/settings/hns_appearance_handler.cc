/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_appearance_handler.h"

#include "base/functional/bind.h"
#include "base/metrics/histogram_macros.h"
#include "base/strings/string_number_conversions.h"
#include "hns/browser/new_tab/new_tab_shows_options.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ntp_background_images/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search/instant_service.h"
#include "chrome/browser/search/instant_service_factory.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"

HnsAppearanceHandler::HnsAppearanceHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kHnsDarkMode,
      base::BindRepeating(&HnsAppearanceHandler::OnHnsDarkModeChanged,
                          base::Unretained(this)));
}

HnsAppearanceHandler::~HnsAppearanceHandler() = default;

// TODO(simonhong): Use separate handler for NTP settings.
void HnsAppearanceHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  profile_state_change_registrar_.Init(profile_->GetPrefs());
  profile_state_change_registrar_.Add(
      kNewTabPageShowsOptions,
      base::BindRepeating(&HnsAppearanceHandler::OnPreferenceChanged,
                          base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePageIsNewTabPage,
      base::BindRepeating(&HnsAppearanceHandler::OnPreferenceChanged,
                          base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePage,
      base::BindRepeating(&HnsAppearanceHandler::OnPreferenceChanged,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setHnsThemeType",
      base::BindRepeating(&HnsAppearanceHandler::SetHnsThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getHnsThemeType",
      base::BindRepeating(&HnsAppearanceHandler::GetHnsThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabShowsOptionsList",
      base::BindRepeating(&HnsAppearanceHandler::GetNewTabShowsOptionsList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "shouldShowNewTabDashboardSettings",
      base::BindRepeating(
          &HnsAppearanceHandler::ShouldShowNewTabDashboardSettings,
          base::Unretained(this)));
}

void HnsAppearanceHandler::SetHnsThemeType(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(args[0].is_int());
  AllowJavascript();

  int int_type = args[0].GetInt();
  dark_mode::SetHnsDarkModeType(
      static_cast<dark_mode::HnsDarkModeType>(int_type));
}

void HnsAppearanceHandler::GetHnsThemeType(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  // GetHnsThemeType() should be used because settings option displays all
  // available options including default.
  ResolveJavascriptCallback(
      args[0],
      base::Value(static_cast<int>(dark_mode::GetHnsDarkModeType())));
}

void HnsAppearanceHandler::OnHnsDarkModeChanged() {
  // GetHnsThemeType() should be used because settings option displays all
  // available options including default.
  if (IsJavascriptAllowed()) {
    FireWebUIListener(
        "hns-theme-type-changed",
        base::Value(static_cast<int>(dark_mode::GetHnsDarkModeType())));
  }
}

void HnsAppearanceHandler::OnBackgroundPreferenceChanged(
    const std::string& pref_name) {
  hns::RecordSponsoredImagesEnabledP3A(profile_);
}

void HnsAppearanceHandler::OnPreferenceChanged(const std::string& pref_name) {
  if (IsJavascriptAllowed()) {
    if (pref_name == kNewTabPageShowsOptions || pref_name == prefs::kHomePage ||
        pref_name == prefs::kHomePageIsNewTabPage) {
      FireWebUIListener(
          "show-new-tab-dashboard-settings-changed",
          base::Value(hns::ShouldNewTabShowDashboard(profile_)));
      return;
    }
  }
}

void HnsAppearanceHandler::GetNewTabShowsOptionsList(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0], base::Value(hns::GetNewTabShowsOptionsList(profile_)));
}

void HnsAppearanceHandler::ShouldShowNewTabDashboardSettings(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0], base::Value(hns::ShouldNewTabShowDashboard(profile_)));
}
