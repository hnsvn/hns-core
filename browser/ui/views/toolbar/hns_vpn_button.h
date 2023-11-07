/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_BUTTON_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_BUTTON_H_

#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#include "chrome/browser/ui/views/toolbar/toolbar_button.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/controls/button/menu_button_controller.h"

namespace hns_vpn {
class HnsVpnService;
}  // namespace hns_vpn

namespace views {
class Border;
}  // namespace views

class Browser;

class HnsVPNButton : public ToolbarButton,
                       public hns_vpn::HnsVPNServiceObserver {
 public:
  METADATA_HEADER(HnsVPNButton);

  explicit HnsVPNButton(Browser* browser);
  ~HnsVPNButton() override;

  HnsVPNButton(const HnsVPNButton&) = delete;
  HnsVPNButton& operator=(const HnsVPNButton&) = delete;

  // HnsVPNServiceObserver overrides:
  void OnConnectionStateChanged(
      hns_vpn::mojom::ConnectionState state) override;
  void OnPurchasedStateChanged(
      hns_vpn::mojom::PurchasedState state,
      const absl::optional<std::string>& description) override;

 private:
  // ToolbarButton overrides:
  void UpdateColorsAndInsets() override;
  std::u16string GetTooltipText(const gfx::Point& p) const override;

  bool IsConnected() const;
  bool IsConnectError() const;
  bool IsPurchased() const;
  std::unique_ptr<views::Border> GetBorder(SkColor border_color) const;
  void OnButtonPressed(const ui::Event& event);

  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<hns_vpn::HnsVpnService> service_ = nullptr;
  raw_ptr<views::MenuButtonController> menu_button_controller_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_BUTTON_H_
