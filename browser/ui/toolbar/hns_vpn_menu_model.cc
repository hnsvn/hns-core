/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/hns_vpn_menu_model.h"

#include "base/feature_list.h"
#include "hns/app/hns_command_ids.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(IS_WIN)
#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"
#endif

HnsVPNMenuModel::HnsVPNMenuModel(Browser* browser,
                                     PrefService* profile_prefs)
    : SimpleMenuModel(this), profile_prefs_(profile_prefs), browser_(browser) {
  Build();
}

HnsVPNMenuModel::~HnsVPNMenuModel() = default;

void HnsVPNMenuModel::Build() {
  AddItemWithStringId(IDC_TOGGLE_HNS_VPN, IDS_HNS_VPN_MENU);
  AddSeparator(ui::NORMAL_SEPARATOR);
  if (!IsHnsVPNButtonVisible()) {
    AddItemWithStringId(IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON,
                        IDS_HNS_VPN_SHOW_VPN_BUTTON_MENU_ITEM);
  }
#if BUILDFLAG(IS_WIN)
  if (!IsTrayIconEnabled()) {
    AddItemWithStringId(IDC_TOGGLE_HNS_VPN_TRAY_ICON,
                        IDS_HNS_VPN_SHOW_VPN_TRAY_ICON_MENU_ITEM);
  }
#endif  // BUILDFLAG(IS_WIN)
  AddItemWithStringId(IDC_SEND_HNS_VPN_FEEDBACK,
                      IDS_HNS_VPN_SHOW_FEEDBACK_MENU_ITEM);
  AddItemWithStringId(IDC_ABOUT_HNS_VPN, IDS_HNS_VPN_ABOUT_VPN_MENU_ITEM);
  AddItemWithStringId(IDC_MANAGE_HNS_VPN_PLAN,
                      IDS_HNS_VPN_MANAGE_MY_PLAN_MENU_ITEM);
}

void HnsVPNMenuModel::ExecuteCommand(int command_id, int event_flags) {
  chrome::ExecuteCommand(browser_, command_id);
}

bool HnsVPNMenuModel::IsHnsVPNButtonVisible() const {
  return profile_prefs_->GetBoolean(hns_vpn::prefs::kHnsVPNShowButton);
}

#if BUILDFLAG(IS_WIN)
bool HnsVPNMenuModel::IsTrayIconEnabled() const {
  if (tray_icon_enabled_for_testing_.has_value()) {
    return tray_icon_enabled_for_testing_.value();
  }

  return hns_vpn::IsVPNTrayIconEnabled();
}
#endif
