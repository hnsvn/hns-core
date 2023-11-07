/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_wallet/wallet_page_ui.h"

#include <string>
#include <utility>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "hns/browser/hns_wallet/asset_ratio_service_factory.h"
#include "hns/browser/hns_wallet/bitcoin_wallet_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_ipfs_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/hns_wallet/swap_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/browser/ui/webui/hns_wallet/wallet_common_ui.h"
#include "hns/browser/ui/webui/navigation_bar_data_provider.h"
#include "hns/components/hns_wallet/browser/asset_ratio_service.h"
#include "hns/components/hns_wallet/browser/blockchain_registry.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_ipfs_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/simulation_service.h"
#include "hns/components/hns_wallet/browser/swap_service.h"
#include "hns/components/hns_wallet/browser/tx_service.h"
#include "hns/components/hns_wallet_page/resources/grit/hns_wallet_page_generated_map.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/sanitized_image_source.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/url_constants.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/webui/web_ui_util.h"

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
#include "hns/browser/hns_wallet/hns_wallet_auto_pin_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_pin_service_factory.h"
#endif

WalletPageUI::WalletPageUI(content::WebUI* web_ui)
    : ui::MojoWebUIController(web_ui,
                              true /* Needed for webui browser tests */) {
  auto* profile = Profile::FromWebUI(web_ui);
  content::WebUIDataSource* source =
      content::WebUIDataSource::CreateAndAdd(profile, kWalletPageHost);
  web_ui->AddRequestableScheme(content::kChromeUIUntrustedScheme);
  for (const auto& str : hns_wallet::kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    source->AddString(str.name, l10n_str);
  }
  NavigationBarDataProvider::Initialize(source, profile);
  webui::SetupWebUIDataSource(
      source,
      base::make_span(kHnsWalletPageGenerated, kHnsWalletPageGeneratedSize),
      IDR_WALLET_PAGE_HTML);
  source->AddString("hnsWalletLedgerBridgeUrl", kUntrustedLedgerURL);
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FrameSrc,
      std::string("frame-src ") + kUntrustedTrezorURL + " " +
          kUntrustedLedgerURL + " " + kUntrustedNftURL + " " +
          kUntrustedMarketURL + ";");
  source->AddString("hnsWalletTrezorBridgeUrl", kUntrustedTrezorURL);
  source->AddString("hnsWalletNftBridgeUrl", kUntrustedNftURL);
  source->AddString("hnsWalletMarketUiBridgeUrl", kUntrustedMarketURL);
  source->AddBoolean(hns_wallet::mojom::kP3ACountTestNetworksLoadTimeKey,
                     base::CommandLine::ForCurrentProcess()->HasSwitch(
                         hns_wallet::mojom::kP3ACountTestNetworksSwitch));
  content::URLDataSource::Add(profile,
                              std::make_unique<SanitizedImageSource>(profile));
  hns_wallet::AddBlockchainTokenImageSource(profile);
}

WalletPageUI::~WalletPageUI() = default;
WEB_UI_CONTROLLER_TYPE_IMPL(WalletPageUI)

void WalletPageUI::BindInterface(
    mojo::PendingReceiver<hns_wallet::mojom::PageHandlerFactory> receiver) {
  page_factory_receiver_.reset();
  page_factory_receiver_.Bind(std::move(receiver));
}

void WalletPageUI::CreatePageHandler(
    mojo::PendingRemote<hns_wallet::mojom::Page> page,
    mojo::PendingReceiver<hns_wallet::mojom::PageHandler> page_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::WalletHandler> wallet_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::JsonRpcService>
        json_rpc_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::BitcoinWalletService>
        bitcoin_rpc_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::SwapService>
        swap_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::AssetRatioService>
        asset_ratio_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::KeyringService>
        keyring_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::BlockchainRegistry>
        blockchain_registry_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::TxService> tx_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::EthTxManagerProxy>
        eth_tx_manager_proxy_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::SolanaTxManagerProxy>
        solana_tx_manager_proxy_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::FilTxManagerProxy>
        filecoin_tx_manager_proxy_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::HnsWalletService>
        hns_wallet_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::HnsWalletP3A>
        hns_wallet_p3a_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::WalletPinService>
        hns_wallet_pin_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::WalletAutoPinService>
        hns_wallet_auto_pin_service_receiver,
    mojo::PendingReceiver<hns_wallet::mojom::IpfsService>
        ipfs_service_receiver) {
  DCHECK(page);
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);

  page_handler_ =
      std::make_unique<WalletPageHandler>(std::move(page_receiver), profile);
  wallet_handler_ = std::make_unique<hns_wallet::WalletHandler>(
      std::move(wallet_receiver), profile);

  hns_wallet::JsonRpcServiceFactory::BindForContext(
      profile, std::move(json_rpc_service_receiver));
  hns_wallet::BitcoinWalletServiceFactory::BindForContext(
      profile, std::move(bitcoin_rpc_service_receiver));
  hns_wallet::SwapServiceFactory::BindForContext(
      profile, std::move(swap_service_receiver));
  hns_wallet::AssetRatioServiceFactory::BindForContext(
      profile, std::move(asset_ratio_service_receiver));
  hns_wallet::KeyringServiceFactory::BindForContext(
      profile, std::move(keyring_service_receiver));
  hns_wallet::TxServiceFactory::BindForContext(
      profile, std::move(tx_service_receiver));
  hns_wallet::TxServiceFactory::BindEthTxManagerProxyForContext(
      profile, std::move(eth_tx_manager_proxy_receiver));
  hns_wallet::TxServiceFactory::BindSolanaTxManagerProxyForContext(
      profile, std::move(solana_tx_manager_proxy_receiver));
  hns_wallet::TxServiceFactory::BindFilTxManagerProxyForContext(
      profile, std::move(filecoin_tx_manager_proxy_receiver));
  hns_wallet::HnsWalletService* wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForContext(profile);
  wallet_service->Bind(std::move(hns_wallet_service_receiver));
  wallet_service->GetHnsWalletP3A()->Bind(
      std::move(hns_wallet_p3a_receiver));

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
  hns_wallet::HnsWalletPinServiceFactory::BindForContext(
      profile, std::move(hns_wallet_pin_service_receiver));
  hns_wallet::HnsWalletAutoPinServiceFactory::BindForContext(
      profile, std::move(hns_wallet_auto_pin_service_receiver));
#endif
  hns_wallet::HnsWalletIpfsServiceFactory::BindForContext(
      profile, std::move(ipfs_service_receiver));

  auto* blockchain_registry = hns_wallet::BlockchainRegistry::GetInstance();
  if (blockchain_registry) {
    blockchain_registry->Bind(std::move(blockchain_registry_receiver));
  }
}
