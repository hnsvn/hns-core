/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WALLET_WALLET_PAGE_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WALLET_WALLET_PAGE_UI_H_

#include <memory>

#include "hns/browser/ui/webui/hns_wallet/common_handler/wallet_handler.h"
#include "hns/browser/ui/webui/hns_wallet/page_handler/wallet_page_handler.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"

#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"

#include "ui/webui/mojo_web_ui_controller.h"

class WalletPageUI : public ui::MojoWebUIController,
                     public hns_wallet::mojom::PageHandlerFactory {
 public:
  explicit WalletPageUI(content::WebUI* web_ui);
  WalletPageUI(const WalletPageUI&) = delete;
  WalletPageUI& operator=(const WalletPageUI&) = delete;
  ~WalletPageUI() override;

  // Instantiates the implementor of the mojom::PageHandlerFactory mojo
  // interface passing the pending receiver that will be internally bound.
  void BindInterface(
      mojo::PendingReceiver<hns_wallet::mojom::PageHandlerFactory> receiver);

 private:
  // hns_wallet::mojom::PageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingRemote<hns_wallet::mojom::Page> page,
      mojo::PendingReceiver<hns_wallet::mojom::PageHandler> page_receiver,
      mojo::PendingReceiver<hns_wallet::mojom::WalletHandler> wallet_receiver,
      mojo::PendingReceiver<hns_wallet::mojom::JsonRpcService>
          json_rpc_service,
      mojo::PendingReceiver<hns_wallet::mojom::BitcoinWalletService>
          bitcoin_rpc_service,
      mojo::PendingReceiver<hns_wallet::mojom::SwapService> swap_service,
      mojo::PendingReceiver<hns_wallet::mojom::AssetRatioService>
          asset_ratio_service,
      mojo::PendingReceiver<hns_wallet::mojom::KeyringService>
          keyring_service,
      mojo::PendingReceiver<hns_wallet::mojom::BlockchainRegistry>
          blockchain_registry,
      mojo::PendingReceiver<hns_wallet::mojom::TxService> tx_service,
      mojo::PendingReceiver<hns_wallet::mojom::EthTxManagerProxy>
          eth_tx_manager_proxy,
      mojo::PendingReceiver<hns_wallet::mojom::SolanaTxManagerProxy>
          solana_tx_manager_proxy,
      mojo::PendingReceiver<hns_wallet::mojom::FilTxManagerProxy>
          filecoin_tx_manager_proxy,
      mojo::PendingReceiver<hns_wallet::mojom::HnsWalletService>
          hns_wallet_service,
      mojo::PendingReceiver<hns_wallet::mojom::HnsWalletP3A>
          hns_wallet_p3a,
      mojo::PendingReceiver<hns_wallet::mojom::WalletPinService>
          hns_wallet_pin_service_receiver,
      mojo::PendingReceiver<hns_wallet::mojom::WalletAutoPinService>
          hns_wallet_auto_pin_service_receiver,
      mojo::PendingReceiver<hns_wallet::mojom::IpfsService>
          hns_wallet_ipfs_service_receiver) override;

  std::unique_ptr<WalletPageHandler> page_handler_;
  std::unique_ptr<hns_wallet::WalletHandler> wallet_handler_;

  mojo::Receiver<hns_wallet::mojom::PageHandlerFactory>
      page_factory_receiver_{this};

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WALLET_WALLET_PAGE_UI_H_
