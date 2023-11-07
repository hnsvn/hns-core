/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_wallet/market/market_ui.h"

#include <string>

#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/components/market_display/resources/grit/market_display_generated_map.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "chrome/grit/browser_resources.h"
#include "chrome/grit/generated_resources.h"
#include "components/grit/hns_components_resources.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/resources/grit/webui_resources.h"

namespace market {

UntrustedMarketUI::UntrustedMarketUI(content::WebUI* web_ui)
    : ui::UntrustedWebUIController(web_ui) {
  auto* untrusted_source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kUntrustedMarketURL);

  for (const auto& str : hns_wallet::kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    untrusted_source->AddString(str.name, l10n_str);
  }
  untrusted_source->SetDefaultResource(IDR_HNS_WALLET_MARKET_DISPLAY_HTML);
  untrusted_source->AddResourcePaths(
      base::make_span(kMarketDisplayGenerated, kMarketDisplayGeneratedSize));
  untrusted_source->AddFrameAncestor(GURL(kHnsUIWalletPageURL));
  untrusted_source->AddFrameAncestor(GURL(kHnsUIWalletPanelURL));
  webui::SetupWebUIDataSource(
      untrusted_source,
      base::make_span(kMarketDisplayGenerated, kMarketDisplayGeneratedSize),
      IDR_HNS_WALLET_MARKET_DISPLAY_HTML);

  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ScriptSrc,
      std::string("script-src 'self' chrome-untrusted://resources;"));

  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      std::string("style-src 'self' 'unsafe-inline';"));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      std::string(
          "style-src 'self' 'unsafe-inline' chrome-untrusted://resources;"));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FontSrc,
      std::string("font-src 'self' data: chrome-untrusted://resources;"));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ImgSrc,
      std::string("img-src 'self' https://assets.cgproxy.hns.com "
                  "chrome-untrusted://resources;"));

  untrusted_source->AddResourcePath("load_time_data_deprecated.js",
                                    IDR_WEBUI_JS_LOAD_TIME_DATA_DEPRECATED_JS);
  untrusted_source->UseStringsJs();
  untrusted_source->AddString("hnsWalletTrezorBridgeUrl",
                              kUntrustedTrezorURL);
  untrusted_source->AddString("hnsWalletLedgerBridgeUrl",
                              kUntrustedLedgerURL);
  untrusted_source->AddString("hnsWalletNftBridgeUrl", kUntrustedNftURL);
  untrusted_source->AddString("hnsWalletMarketUiBridgeUrl",
                              kUntrustedMarketURL);
}

UntrustedMarketUI::~UntrustedMarketUI() = default;

std::unique_ptr<content::WebUIController>
UntrustedMarketUIConfig::CreateWebUIController(content::WebUI* web_ui,
                                               const GURL& url) {
  return std::make_unique<UntrustedMarketUI>(web_ui);
}

UntrustedMarketUIConfig::UntrustedMarketUIConfig()
    : WebUIConfig(content::kChromeUIUntrustedScheme, kUntrustedMarketHost) {}

}  // namespace market
