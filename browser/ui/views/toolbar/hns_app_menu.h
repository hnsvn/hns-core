/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_APP_MENU_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_APP_MENU_H_

#include "base/memory/raw_ptr.h"
#include "hns/components/misc_metrics/menu_metrics.h"
#include "chrome/browser/ui/views/toolbar/app_menu.h"

class HnsAppMenu : public AppMenu {
 public:
  HnsAppMenu(Browser* browser, ui::MenuModel* model, int run_types);
  ~HnsAppMenu() override;

  HnsAppMenu(const HnsAppMenu&) = delete;
  HnsAppMenu& operator=(const HnsAppMenu&) = delete;

  // AppMenu overrides:
  void RunMenu(views::MenuButtonController* host) override;
  void ExecuteCommand(int command_id, int mouse_event_flags) override;
  void OnMenuClosed(views::MenuItemView* menu) override;

 private:
  void RecordMenuUsage(int command_id);

  // Update item's view as hns style.
  void UpdateMenuItemView();

  base::raw_ptr<misc_metrics::MenuMetrics> menu_metrics_;
};

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_APP_MENU_H_
