/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_PANEL_CONTROLLER_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_PANEL_CONTROLLER_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "hns/browser/ui/webui/hns_vpn/vpn_panel_ui.h"
#include "chrome/browser/ui/views/bubble/webui_bubble_manager.h"

class HnsBrowserView;

class HnsVPNPanelController {
 public:
  explicit HnsVPNPanelController(HnsBrowserView* browser_view);
  ~HnsVPNPanelController();
  HnsVPNPanelController(const HnsVPNPanelController&) = delete;
  HnsVPNPanelController& operator=(const HnsVPNPanelController&) = delete;

  void ShowHnsVPNPanel();
  // Manager should be reset to use different anchor view for bubble.
  void ResetBubbleManager();

 private:
  raw_ptr<HnsBrowserView> browser_view_ = nullptr;
  std::unique_ptr<WebUIBubbleManagerT<VPNPanelUI>> webui_bubble_manager_;
};

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_PANEL_CONTROLLER_H_
