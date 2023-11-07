/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TOOLBAR_HNS_VPN_MENU_MODEL_H_
#define HNS_BROWSER_UI_TOOLBAR_HNS_VPN_MENU_MODEL_H_

#include "base/memory/raw_ptr.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class PrefService;

class HnsVPNMenuModel : public ui::SimpleMenuModel,
                          public ui::SimpleMenuModel::Delegate {
 public:
  HnsVPNMenuModel(Browser* browser, PrefService* profile_prefs);
  ~HnsVPNMenuModel() override;

  HnsVPNMenuModel(const HnsVPNMenuModel&) = delete;
  HnsVPNMenuModel& operator=(const HnsVPNMenuModel&) = delete;
#if BUILDFLAG(IS_WIN)
  void SetTrayIconEnabledForTesting(bool value) {
    tray_icon_enabled_for_testing_ = value;
  }
#endif  // BUILDFLAG(IS_WIN)
 private:
  FRIEND_TEST_ALL_PREFIXES(HnsVPNMenuModelUnitTest, TrayIconEnabled);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNMenuModelUnitTest, TrayIconDisabled);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNMenuModelUnitTest, ToolbarVPNButton);

  // ui::SimpleMenuModel::Delegate override:
  void ExecuteCommand(int command_id, int event_flags) override;

  void Build();
  bool IsHnsVPNButtonVisible() const;
#if BUILDFLAG(IS_WIN)
  bool IsTrayIconEnabled() const;
#endif  // BUILDFLAG(IS_WIN)
  absl::optional<bool> tray_icon_enabled_for_testing_;
  raw_ptr<PrefService> profile_prefs_ = nullptr;
  raw_ptr<Browser> browser_ = nullptr;
};

#endif  // HNS_BROWSER_UI_TOOLBAR_HNS_VPN_MENU_MODEL_H_
