// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/hns_wallet/android/android_wallet_page_ui.h"

#include <utility>

#include "hns/components/hns_wallet_page/resources/grit/hns_wallet_deposit_page_generated_map.h"
#include "hns/components/hns_wallet_page/resources/grit/hns_wallet_fund_wallet_page_generated_map.h"
#include "hns/components/hns_wallet_page/resources/grit/hns_wallet_send_page_generated_map.h"
#include "hns/components/hns_wallet_page/resources/grit/hns_wallet_swap_page_generated_map.h"
#include "hns/components/l10n/common/localization_util.h"

#include "hns/browser/ui/webui/hns_wallet/wallet_common_ui.h"
#include "hns/browser/ui/webui/hns_webui_source.h"

#include "hns/browser/hns_wallet/asset_ratio_service_factory.h"
#include "hns/browser/hns_wallet/bitcoin_wallet_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_ipfs_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_provider_delegate_impl_helper.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/hns_wallet/swap_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"

#include "hns/components/hns_wallet/browser/blockchain_registry.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"

#include "hns/components/constants/webui_url_constants.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/common/url_constants.h"

#include "base/command_line.h"

AndroidWalletPageUI::AndroidWalletPageUI(content::WebUI* web_ui,
                                         const GURL& url)
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

  // Add required resources.
  if (url.path() == kWalletSwapPagePath) {
    webui::SetupWebUIDataSource(
        source,
        base::make_span(kHnsWalletSwapPageGenerated,
                        kHnsWalletSwapPageGeneratedSize),
        IDR_HNS_WALLET_SWAP_PAGE_HTML);
  } else if (url.path() == kWalletSendPagePath) {
    webui::SetupWebUIDataSource(
        source,
        base::make_span(kHnsWalletSendPageGenerated,
                        kHnsWalletSendPageGeneratedSize),
        IDR_HNS_WALLET_SEND_PAGE_HTML);

  } else if (url.path() == kWalletBuyPagePath) {
    webui::SetupWebUIDataSource(
        source,
        base::make_span(kHnsWalletFundWalletPageGenerated,
                        kHnsWalletFundWalletPageGeneratedSize),
        IDR_HNS_WALLET_FUND_WALLET_PAGE_HTML);

  } else if (url.path() == kWalletDepositPagePath) {
    webui::SetupWebUIDataSource(
        source,
        base::make_span(kHnsWalletDepositPageGenerated,
                        kHnsWalletDepositPageGeneratedSize),
        IDR_HNS_WALLET_DEPOSIT_PAGE_HTML);
  } else {
    NOTREACHED() << "Failed to find page resources for:" << url.path();
  }

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

  hns_wallet::AddBlockchainTokenImageSource(profile);
}

AndroidWalletPageUI::~AndroidWalletPageUI() = default;

void AndroidWalletPageUI::BindInterface(
    mojo::PendingReceiver<hns_wallet::mojom::PageHandlerFactory> receiver) {
  page_factory_receiver_.reset();
  page_factory_receiver_.Bind(std::move(receiver));
}

WEB_UI_CONTROLLER_TYPE_IMPL(AndroidWalletPageUI)

void AndroidWalletPageUI::CreatePageHandler(
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
  page_handler_ = std::make_unique<AndroidWalletPageHandler>(
      std::move(page_receiver), profile, this);
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
  hns_wallet::HnsWalletIpfsServiceFactory::BindForContext(
      profile, std::move(ipfs_service_receiver));
  hns_wallet::HnsWalletService* wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForContext(profile);
  wallet_service->Bind(std::move(hns_wallet_service_receiver));
  wallet_service->GetHnsWalletP3A()->Bind(
      std::move(hns_wallet_p3a_receiver));

  auto* blockchain_registry = hns_wallet::BlockchainRegistry::GetInstance();
  if (blockchain_registry) {
    blockchain_registry->Bind(std::move(blockchain_registry_receiver));
  }
  hns_wallet::WalletInteractionDetected(web_ui()->GetWebContents());
}
