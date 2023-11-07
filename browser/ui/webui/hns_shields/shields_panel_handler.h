// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_SHIELDS_PANEL_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_SHIELDS_PANEL_HANDLER_H_

#include "hns/components/hns_shields/common/hns_shields_panel.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace ui {
class MojoBubbleWebUIController;
}  // namespace ui

namespace content {
class WebUI;
}  // namespace content

class Profile;
class HnsBrowserWindow;

class ShieldsPanelHandler : public hns_shields::mojom::PanelHandler {
 public:
  ShieldsPanelHandler(
      mojo::PendingReceiver<hns_shields::mojom::PanelHandler> receiver,
      ui::MojoBubbleWebUIController* webui_controller,
      HnsBrowserWindow* hns_browser_window,
      Profile* profile);

  ShieldsPanelHandler(const ShieldsPanelHandler&) = delete;
  ShieldsPanelHandler& operator=(const ShieldsPanelHandler&) = delete;
  ~ShieldsPanelHandler() override;

  // hns_shields::mojom::PanelHandler:
  void ShowUI() override;
  void CloseUI() override;
  void GetPosition(GetPositionCallback callback) override;
  void SetAdvancedViewEnabled(bool is_enabled) override;
  void GetAdvancedViewEnabled(GetAdvancedViewEnabledCallback callback) override;

 private:
  mojo::Receiver<hns_shields::mojom::PanelHandler> receiver_;
  raw_ptr<ui::MojoBubbleWebUIController> const webui_controller_;
  raw_ptr<HnsBrowserWindow> hns_browser_window_ = nullptr;
  raw_ptr<Profile> profile_ = nullptr;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_SHIELDS_PANEL_HANDLER_H_
