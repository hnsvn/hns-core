/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/toolbar/hns_app_menu.h"

#include <memory>

#include "hns/app/hns_command_ids.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/misc_metrics/process_misc_metrics.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/misc_metrics/menu_metrics.h"
#include "chrome/app/chrome_command_ids.h"
#include "ui/base/models/menu_model.h"
#include "ui/views/controls/menu/menu_item_view.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/ui/views/toolbar/hns_vpn_status_label.h"
#include "hns/browser/ui/views/toolbar/hns_vpn_toggle_button.h"
#endif

using views::MenuItemView;

HnsAppMenu::HnsAppMenu(Browser* browser,
                           ui::MenuModel* model,
                           int run_types)
    : AppMenu(browser, model, run_types),
      menu_metrics_(
          g_hns_browser_process->process_misc_metrics()->menu_metrics()) {
  DCHECK(menu_metrics_);
  UpdateMenuItemView();
}

HnsAppMenu::~HnsAppMenu() = default;

void HnsAppMenu::RunMenu(views::MenuButtonController* host) {
  AppMenu::RunMenu(host);
  menu_metrics_->RecordMenuShown();
}

void HnsAppMenu::ExecuteCommand(int command_id, int mouse_event_flags) {
  AppMenu::ExecuteCommand(command_id, mouse_event_flags);
  RecordMenuUsage(command_id);
}

void HnsAppMenu::OnMenuClosed(views::MenuItemView* menu) {
  AppMenu::OnMenuClosed(menu);
  if (menu == nullptr) {
    menu_metrics_->RecordMenuDismiss();
  }
}

void HnsAppMenu::RecordMenuUsage(int command_id) {
  misc_metrics::MenuGroup group;

  switch (command_id) {
    case IDC_NEW_WINDOW:
    case IDC_NEW_TAB:
    case IDC_NEW_INCOGNITO_WINDOW:
    case IDC_NEW_OFFTHERECORD_WINDOW_TOR:
    case IDC_OPEN_GUEST_PROFILE:
      group = misc_metrics::MenuGroup::kTabWindow;
      break;
    case IDC_SHOW_HNS_WALLET:
    case IDC_SHOW_HNS_SYNC:
    case IDC_SHOW_HNS_REWARDS:
      group = misc_metrics::MenuGroup::kHnsFeatures;
      break;
    case IDC_SHOW_HISTORY:
    case IDC_MANAGE_EXTENSIONS:
    case IDC_SHOW_BOOKMARK_MANAGER:
    case IDC_BOOKMARK_THIS_TAB:
    case IDC_BOOKMARK_ALL_TABS:
    case IDC_SHOW_BOOKMARK_BAR:
    case IDC_IMPORT_SETTINGS:
    case IDC_OPTIONS:
    case IDC_SHOW_DOWNLOADS:
      group = misc_metrics::MenuGroup::kBrowserViews;
      break;
    default:
      if (command_id >= IDC_FIRST_UNBOUNDED_MENU) {
        group = misc_metrics::MenuGroup::kBrowserViews;
      } else {
        return;
      }
  }
  menu_metrics_->RecordMenuGroupAction(group);
}

void HnsAppMenu::UpdateMenuItemView() {
#if BUILDFLAG(ENABLE_HNS_VPN)
  CHECK(root_menu_item());
  if (auto* menu_item =
          root_menu_item()->GetMenuItemByID(IDC_TOGGLE_HNS_VPN)) {
    menu_item->AddChildView(std::make_unique<HnsVPNStatusLabel>(browser_));
    menu_item->AddChildView(std::make_unique<HnsVPNToggleButton>(browser_));
  }
#endif
}
