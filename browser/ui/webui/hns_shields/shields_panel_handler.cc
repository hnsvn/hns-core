// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/hns_shields/shields_panel_handler.h"

#include <utility>

#include "hns/browser/ui/hns_browser_window.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "ui/gfx/geometry/vector2d.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

ShieldsPanelHandler::ShieldsPanelHandler(
    mojo::PendingReceiver<hns_shields::mojom::PanelHandler> receiver,
    ui::MojoBubbleWebUIController* webui_controller,
    HnsBrowserWindow* hns_browser_window,
    Profile* profile)
    : receiver_(this, std::move(receiver)),
      webui_controller_(webui_controller),
      hns_browser_window_(hns_browser_window),
      profile_(profile) {}

ShieldsPanelHandler::~ShieldsPanelHandler() = default;

void ShieldsPanelHandler::ShowUI() {
  auto embedder = webui_controller_->embedder();
  if (embedder) {
    embedder->ShowUI();
  }
}

void ShieldsPanelHandler::CloseUI() {
  auto embedder = webui_controller_->embedder();
  if (embedder) {
    embedder->CloseUI();
  }
}

void ShieldsPanelHandler::GetPosition(GetPositionCallback callback) {
  gfx::Vector2d vec =
      gfx::Vector2d(hns_browser_window_->GetShieldsBubbleRect().x(),
                    hns_browser_window_->GetShieldsBubbleRect().y());
  std::move(callback).Run(vec);
}

void ShieldsPanelHandler::SetAdvancedViewEnabled(bool is_enabled) {
  DCHECK(profile_);

  profile_->GetPrefs()->SetBoolean(kShieldsAdvancedViewEnabled, is_enabled);
}

void ShieldsPanelHandler::GetAdvancedViewEnabled(
    GetAdvancedViewEnabledCallback callback) {
  DCHECK(profile_);

  bool is_enabled =
      profile_->GetPrefs()->GetBoolean(kShieldsAdvancedViewEnabled);
  std::move(callback).Run(is_enabled);
}
