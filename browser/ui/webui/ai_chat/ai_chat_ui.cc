// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/ai_chat/ai_chat_ui.h"

#include <utility>

#include "hns/browser/ui/side_panel/ai_chat/ai_chat_side_panel_utils.h"
#include "hns/browser/ui/webui/ai_chat/ai_chat_ui_page_handler.h"
#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/components/ai_chat/browser/constants.h"
#include "hns/components/ai_chat/common/pref_names.h"
#include "hns/components/ai_chat/resources/page/grit/ai_chat_ui_generated_map.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/grit/hns_components_resources.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/url_constants.h"

AIChatUI::AIChatUI(content::WebUI* web_ui)
    : ui::UntrustedWebUIController(web_ui),
      profile_(Profile::FromWebUI(web_ui)) {
  DCHECK(profile_);

  // Create a URLDataSource and add resources.
  content::WebUIDataSource* untrusted_source =
      content::WebUIDataSource::CreateAndAdd(
          web_ui->GetWebContents()->GetBrowserContext(), kChatUIURL);

  webui::SetupWebUIDataSource(
      untrusted_source,
      base::make_span(kAiChatUiGenerated, kAiChatUiGeneratedSize),
      IDR_CHAT_UI_HTML);

  untrusted_source->AddResourcePath("styles.css", IDR_CHAT_UI_CSS);

  for (const auto& str : ai_chat::GetLocalizedStrings()) {
    untrusted_source->AddString(
        str.name, hns_l10n::GetLocalizedResourceUTF16String(str.id));
  }

  untrusted_source->AddBoolean(
      "hasSeenAgreement",
      profile_->GetOriginalProfile()->GetPrefs()->GetBoolean(
          ai_chat::prefs::kHnsChatHasSeenDisclaimer));

  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ScriptSrc,
      "script-src 'self' chrome-untrusted://resources;");
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      "style-src 'self' 'unsafe-inline' chrome-untrusted://resources;");
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ImgSrc,
      "img-src 'self' blob: chrome-untrusted://resources;");
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FontSrc,
      "font-src 'self' data: chrome-untrusted://resources;");
}

AIChatUI::~AIChatUI() = default;

void AIChatUI::BindInterface(
    mojo::PendingReceiver<ai_chat::mojom::PageHandler> receiver) {
  // We call ShowUI() before creating the PageHandler object so that
  // the WebContents is added to a Browser which we can get a reference
  // to and provide to the PageHandler.
  if (embedder_) {
    embedder_->ShowUI();
  }

  browser_ = ai_chat::GetBrowserForWebContents(web_ui()->GetWebContents());
  DCHECK(browser_);

  page_handler_ = std::make_unique<ai_chat::AIChatUIPageHandler>(
      web_ui()->GetWebContents(), browser_->tab_strip_model(), profile_,
      std::move(receiver));
}

std::unique_ptr<content::WebUIController>
UntrustedChatUIConfig::CreateWebUIController(content::WebUI* web_ui,
                                             const GURL& url) {
  return std::make_unique<AIChatUI>(web_ui);
}

UntrustedChatUIConfig::UntrustedChatUIConfig()
    : WebUIConfig(content::kChromeUIUntrustedScheme, kChatUIHost) {}

WEB_UI_CONTROLLER_TYPE_IMPL(AIChatUI)
