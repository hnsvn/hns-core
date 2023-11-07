// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_VPN_VPN_PANEL_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_VPN_VPN_PANEL_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "chrome/browser/profiles/profile.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

namespace content {
class WebUI;
}  // namespace content

class VPNPanelUI;

class VPNPanelHandler : public hns_vpn::mojom::PanelHandler {
 public:
  using GetWebContentsForTabCallback =
      base::RepeatingCallback<content::WebContents*(int32_t)>;

  VPNPanelHandler(
      mojo::PendingReceiver<hns_vpn::mojom::PanelHandler> receiver,
      VPNPanelUI* panel_controller,
      Profile* profile);

  VPNPanelHandler(const VPNPanelHandler&) = delete;
  VPNPanelHandler& operator=(const VPNPanelHandler&) = delete;
  ~VPNPanelHandler() override;

  // hns_vpn::mojom::PanelHandler:
  void ShowUI() override;
  void CloseUI() override;
  void OpenVpnUI(const std::string& type) override;

 private:
  void OpenVpnUIUrl(const std::string& type,
                    hns_vpn::mojom::ProductUrlsPtr product_urls);
  mojo::Receiver<hns_vpn::mojom::PanelHandler> receiver_;
  raw_ptr<VPNPanelUI> const panel_controller_;
  raw_ptr<Profile> profile_;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_VPN_VPN_PANEL_HANDLER_H_
