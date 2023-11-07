/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_VPN_HNS_VPN_DNS_SETTINGS_NOTIFICIATION_DIALOG_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_VPN_HNS_VPN_DNS_SETTINGS_NOTIFICIATION_DIALOG_VIEW_H_

#include "ui/views/window/dialog_delegate.h"

class Browser;
class PrefService;

namespace views {
class Checkbox;
}

namespace hns_vpn {

class HnsVpnDnsSettingsNotificiationDialogView
    : public views::DialogDelegateView {
 public:
  METADATA_HEADER(HnsVpnDnsSettingsNotificiationDialogView);

  static void Show(Browser* browser);

  HnsVpnDnsSettingsNotificiationDialogView(
      const HnsVpnDnsSettingsNotificiationDialogView&) = delete;
  HnsVpnDnsSettingsNotificiationDialogView& operator=(
      const HnsVpnDnsSettingsNotificiationDialogView&) = delete;

 private:
  explicit HnsVpnDnsSettingsNotificiationDialogView(Browser* browser);
  ~HnsVpnDnsSettingsNotificiationDialogView() override;

  void OnAccept();
  void OnClosing();

  void OnLearnMoreLinkClicked();

  // views::DialogDelegate overrides:
  ui::ModalType GetModalType() const override;
  bool ShouldShowCloseButton() const override;
  bool ShouldShowWindowTitle() const override;

  bool close_window_ = true;
  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<PrefService> prefs_ = nullptr;
  raw_ptr<views::Checkbox> dont_ask_again_checkbox_ = nullptr;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_UI_VIEWS_HNS_VPN_HNS_VPN_DNS_SETTINGS_NOTIFICIATION_DIALOG_VIEW_H_
