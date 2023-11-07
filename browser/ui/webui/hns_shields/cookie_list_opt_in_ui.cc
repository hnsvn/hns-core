/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_shields/cookie_list_opt_in_ui.h"

#include <memory>
#include <utility>

#include "base/feature_list.h"
#include "hns/browser/ui/webui/hns_shields/cookie_list_opt_in_page_handler.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_shields/resources/cookie_list_opt_in/grit/cookie_list_opt_in_generated_map.h"
#include "hns/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/favicon_base/favicon_url_parser.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"

namespace {

static constexpr webui::LocalizedString kStrings[] = {
    {"cookieListTitle", IDS_HNS_SHIELDS_COOKIE_LIST_TITLE},
    {"cookieListHeader", IDS_HNS_SHIELDS_COOKIE_LIST_HEADER},
    {"cookieListText", IDS_HNS_SHIELDS_COOKIE_LIST_TEXT},
    {"cookieListButtonText", IDS_HNS_SHIELDS_COOKIE_LIST_BUTTON_TEXT},
    {"cookieListNoThanks", IDS_HNS_SHIELDS_COOKIE_LIST_NO_THANKS}};

}  // namespace

CookieListOptInUI::CookieListOptInUI(content::WebUI* web_ui)
    : MojoBubbleWebUIController(web_ui, true) {
  DCHECK(base::FeatureList::IsEnabled(
      hns_shields::features::kHnsAdblockCookieListOptIn));

  auto* profile = Profile::FromWebUI(web_ui);

  auto* source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kCookieListOptInHost);
  source->AddLocalizedStrings(kStrings);

  webui::SetupWebUIDataSource(
      source,
      base::make_span(kCookieListOptInGenerated, kCookieListOptInGeneratedSize),
      IDR_COOKIE_LIST_OPT_IN_HTML);

  content::URLDataSource::Add(
      profile, std::make_unique<FaviconSource>(
                   profile, chrome::FaviconUrlFormat::kFavicon2));
}

CookieListOptInUI::~CookieListOptInUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(CookieListOptInUI)

void CookieListOptInUI::BindInterface(
    mojo::PendingReceiver<CookieListOptInPageHandlerFactory> receiver) {
  page_factory_receiver_.reset();
  page_factory_receiver_.Bind(std::move(receiver));
}

void CookieListOptInUI::CreatePageHandler(
    mojo::PendingReceiver<hns_shields::mojom::CookieListOptInPageHandler>
        receiver) {
  page_handler_ = std::make_unique<CookieListOptInPageHandler>(
      std::move(receiver), embedder(), Profile::FromWebUI(web_ui()));
}
