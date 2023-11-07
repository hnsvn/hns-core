/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_REWARDS_REWARDS_PANEL_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_REWARDS_REWARDS_PANEL_UI_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_rewards/common/mojom/rewards_panel.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

namespace hns_rewards {

class RewardsPanelCoordinator;

class RewardsPanelUI : public ui::MojoBubbleWebUIController,
                       public mojom::PanelHandlerFactory {
 public:
  explicit RewardsPanelUI(content::WebUI* web_ui);
  ~RewardsPanelUI() override;

  RewardsPanelUI(const RewardsPanelUI&) = delete;
  RewardsPanelUI& operator=(const RewardsPanelUI&) = delete;

  void BindInterface(mojo::PendingReceiver<PanelHandlerFactory> receiver);

 private:
  // mojom::PanelHandlerFactory:
  void CreatePanelHandler(
      mojo::PendingRemote<mojom::Panel> panel,
      mojo::PendingReceiver<mojom::PanelHandler> receiver) override;

  std::unique_ptr<mojom::PanelHandler> panel_handler_;
  mojo::Receiver<PanelHandlerFactory> panel_factory_receiver_{this};
  raw_ptr<RewardsPanelCoordinator> panel_coordinator_ = nullptr;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

}  // namespace hns_rewards

#endif  // HNS_BROWSER_UI_WEBUI_HNS_REWARDS_REWARDS_PANEL_UI_H_
