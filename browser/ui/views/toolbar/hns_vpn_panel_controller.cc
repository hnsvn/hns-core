/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/toolbar/hns_vpn_panel_controller.h"

#include "hns/browser/ui/views/bubble/hns_webui_bubble_manager.h"
#include "hns/browser/ui/views/frame/hns_browser_view.h"
#include "hns/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "components/grit/hns_components_strings.h"
#include "url/gurl.h"

HnsVPNPanelController::HnsVPNPanelController(HnsBrowserView* browser_view)
    : browser_view_(browser_view) {
  DCHECK(browser_view_);
}

HnsVPNPanelController::~HnsVPNPanelController() = default;

void HnsVPNPanelController::ShowHnsVPNPanel() {
  auto* anchor_view = browser_view_->GetAnchorViewForHnsVPNPanel();
  if (!anchor_view)
    return;

  if (!webui_bubble_manager_) {
    auto* profile = browser_view_->browser()->profile();
    webui_bubble_manager_ =
        std::make_unique<HnsWebUIBubbleManager<VPNPanelUI>>(
            anchor_view, profile, GURL(kVPNPanelURL), IDS_HNS_VPN_PANEL_NAME);
  }

  if (webui_bubble_manager_->GetBubbleWidget()) {
    webui_bubble_manager_->CloseBubble();
    return;
  }

  webui_bubble_manager_->ShowBubble();
}

void HnsVPNPanelController::ResetBubbleManager() {
  webui_bubble_manager_.reset();
}
