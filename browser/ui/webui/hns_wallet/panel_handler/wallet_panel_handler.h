// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WALLET_PANEL_HANDLER_WALLET_PANEL_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WALLET_PANEL_HANDLER_WALLET_PANEL_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

namespace content {
class WebUI;
}

class WalletPanelHandler : public hns_wallet::mojom::PanelHandler {
 public:
  using PanelCloseOnDeactivationCallback = base::RepeatingCallback<void(bool)>;
  WalletPanelHandler(
      mojo::PendingReceiver<hns_wallet::mojom::PanelHandler> receiver,
      ui::MojoBubbleWebUIController* webui_controller,
      content::WebContents* active_web_contents,
      PanelCloseOnDeactivationCallback close_on_deactivation);

  WalletPanelHandler(const WalletPanelHandler&) = delete;
  WalletPanelHandler& operator=(const WalletPanelHandler&) = delete;
  ~WalletPanelHandler() override;

  // hns_wallet::mojom::PanelHandler:
  void ShowUI() override;
  void CloseUI() override;
  void SetCloseOnDeactivate(bool close) override;
  void ConnectToSite(
      const std::vector<std::string>& accounts,
      hns_wallet::mojom::PermissionLifetimeOption option) override;
  void CancelConnectToSite() override;
  void Focus() override;
  void IsSolanaAccountConnected(
      const std::string& account,
      IsSolanaAccountConnectedCallback callback) override;

 private:
  mojo::Receiver<hns_wallet::mojom::PanelHandler> receiver_;
  const raw_ptr<ui::MojoBubbleWebUIController> webui_controller_;
  raw_ptr<content::WebContents> active_web_contents_ = nullptr;
  const PanelCloseOnDeactivationCallback close_on_deactivation_;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WALLET_PANEL_HANDLER_WALLET_PANEL_HANDLER_H_
