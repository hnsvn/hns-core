/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_REWARDS_REWARDS_PANEL_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_REWARDS_REWARDS_PANEL_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/browser/ui/hns_rewards/rewards_panel_coordinator.h"
#include "hns/components/hns_rewards/common/mojom/rewards_panel.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

namespace hns_rewards {

class RewardsService;

class RewardsPanelHandler : public mojom::PanelHandler,
                            public RewardsPanelCoordinator::Observer {
 public:
  RewardsPanelHandler(
      mojo::PendingRemote<mojom::Panel> panel,
      mojo::PendingReceiver<mojom::PanelHandler> receiver,
      base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder,
      RewardsService* rewards_service,
      RewardsPanelCoordinator* panel_coordinator);

  RewardsPanelHandler(const RewardsPanelHandler&) = delete;
  RewardsPanelHandler& operator=(const RewardsPanelHandler&) = delete;

  ~RewardsPanelHandler() override;

  // mojom::PanelHandler:
  void ShowUI() override;
  void CloseUI() override;
  void GetRewardsPanelArgs(GetRewardsPanelArgsCallback callback) override;

  // RewardsPanelCoordinator::Observer:
  void OnRewardsPanelRequested(const mojom::RewardsPanelArgs& args) override;

 private:
  mojo::Receiver<mojom::PanelHandler> receiver_;
  mojo::Remote<mojom::Panel> panel_;
  base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder_;
  raw_ptr<RewardsService> rewards_service_ = nullptr;
  raw_ptr<RewardsPanelCoordinator> panel_coordinator_ = nullptr;
  RewardsPanelCoordinator::Observation panel_observation_{this};
};

}  // namespace hns_rewards

#endif  // HNS_BROWSER_UI_WEBUI_HNS_REWARDS_REWARDS_PANEL_HANDLER_H_
