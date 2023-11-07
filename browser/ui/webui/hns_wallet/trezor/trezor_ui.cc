/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_wallet/trezor/trezor_ui.h"

#include <string>

#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/trezor_bridge/resources/grit/trezor_bridge_generated_map.h"
#include "components/grit/hns_components_resources.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/resources/grit/webui_resources.h"

namespace {
const char kTrezorConnectURL[] = "https://connect.trezor.io/";
}  // namespace

namespace trezor {

UntrustedTrezorUI::UntrustedTrezorUI(content::WebUI* web_ui)
    : ui::UntrustedWebUIController(web_ui) {
  auto* untrusted_source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kUntrustedTrezorURL);
  untrusted_source->SetDefaultResource(IDR_HNS_WALLET_TREZOR_BRIDGE_HTML);
  untrusted_source->AddResourcePaths(
      base::make_span(kTrezorBridgeGenerated, kTrezorBridgeGeneratedSize));
  untrusted_source->AddFrameAncestor(GURL(kHnsUIWalletPageURL));
  untrusted_source->AddFrameAncestor(GURL(kHnsUIWalletPanelURL));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ScriptSrc,
      std::string("script-src chrome://resources/js/ 'self' ") +
          kTrezorConnectURL + ";");
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FrameSrc,
      std::string("frame-src ") + kTrezorConnectURL + ";");
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      std::string("style-src 'unsafe-inline';"));
  untrusted_source->AddResourcePath("load_time_data_deprecated.js",
                                    IDR_WEBUI_JS_LOAD_TIME_DATA_DEPRECATED_JS);
  untrusted_source->UseStringsJs();
  untrusted_source->AddString("hnsWalletTrezorBridgeUrl",
                              kUntrustedTrezorURL);
}

UntrustedTrezorUI::~UntrustedTrezorUI() = default;

std::unique_ptr<content::WebUIController>
UntrustedTrezorUIConfig::CreateWebUIController(content::WebUI* web_ui,
                                               const GURL& url) {
  return std::make_unique<UntrustedTrezorUI>(web_ui);
}

UntrustedTrezorUIConfig::UntrustedTrezorUIConfig()
    : WebUIConfig(content::kChromeUIUntrustedScheme, kUntrustedTrezorHost) {}

}  // namespace trezor
