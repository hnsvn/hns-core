/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_TOGGLE_BUTTON_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_TOGGLE_BUTTON_H_

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#include "ui/views/controls/button/toggle_button.h"

namespace hns_vpn {
class HnsVpnService;
}  // namespace hns_vpn

class Browser;

class HnsVPNToggleButton : public views::ToggleButton,
                             public hns_vpn::HnsVPNServiceObserver {
 public:
  explicit HnsVPNToggleButton(Browser* browser);
  ~HnsVPNToggleButton() override;

  HnsVPNToggleButton(const HnsVPNToggleButton&) = delete;
  HnsVPNToggleButton& operator=(const HnsVPNToggleButton&) = delete;

 private:
  // HnsVPNServiceObserver overrides:
  void OnConnectionStateChanged(
      hns_vpn::mojom::ConnectionState state) override;

  void OnButtonPressed(const ui::Event& event);
  void UpdateState();

  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<hns_vpn::HnsVpnService> service_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_TOGGLE_BUTTON_H_
