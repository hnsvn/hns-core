// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_ui.h"

#include <utility>

#include "base/check.h"
#include "base/feature_list.h"
#include "hns/browser/hns_news/hns_news_controller_factory.h"
#include "hns/browser/new_tab/new_tab_shows_options.h"
#include "hns/browser/ntp_background/hns_ntp_custom_background_service_factory.h"
#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_message_handler.h"
#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_page_handler.h"
#include "hns/browser/ui/webui/new_tab_page/top_sites_message_handler.h"
#include "hns/components/hns_new_tab/resources/grit/hns_new_tab_generated_map.h"
#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "hns/components/hns_news/common/features.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/components/ntp_background_images/browser/ntp_custom_images_source.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/common/pref_names.h"
#include "components/grit/hns_components_resources.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"

using ntp_background_images::NTPCustomImagesSource;

HnsNewTabUI::HnsNewTabUI(content::WebUI* web_ui, const std::string& name)
    : ui::MojoWebUIController(
          web_ui,
          true /* Needed for legacy non-mojom message handler */),
      page_factory_receiver_(this) {
  content::WebContents* web_contents = web_ui->GetWebContents();
  CHECK(web_contents);

  content::NavigationEntry* navigation_entry =
      web_contents->GetController().GetLastCommittedEntry();
  const bool was_restored =
      navigation_entry ? navigation_entry->IsRestored() : false;

  const bool is_visible =
      web_contents->GetVisibility() == content::Visibility::VISIBLE;

  Profile* profile = Profile::FromWebUI(web_ui);
  web_ui->OverrideTitle(
      hns_l10n::GetLocalizedResourceUTF16String(IDS_NEW_TAB_TITLE));

  if (hns::ShouldNewTabShowBlankpage(profile)) {
    content::WebUIDataSource* source =
        content::WebUIDataSource::CreateAndAdd(profile, name);
    source->SetDefaultResource(IDR_HNS_BLANK_NEW_TAB_HTML);
    AddBackgroundColorToSource(source, web_contents);
    return;
  }

  // Non blank NTP.
  content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
      web_ui, name, kHnsNewTabGenerated, kHnsNewTabGeneratedSize,
      IDR_HNS_NEW_TAB_HTML);

  AddBackgroundColorToSource(source, web_contents);

  source->AddBoolean("featureCustomBackgroundEnabled",
                     !profile->GetPrefs()->IsManagedPreference(
                         prefs::kNtpCustomBackgroundDict));

  // Let frontend know about feature flags
  source->AddBoolean("featureFlagHnsNewsPromptEnabled",
                     base::FeatureList::IsEnabled(
                         hns_news::features::kHnsNewsCardPeekFeature));

  web_ui->AddMessageHandler(base::WrapUnique(HnsNewTabMessageHandler::Create(
      source, profile, was_restored && !is_visible)));
  web_ui->AddMessageHandler(
      base::WrapUnique(new TopSitesMessageHandler(profile)));

  // For custom background images.
  if (auto* ntp_custom_background_images_service =
          HnsNTPCustomBackgroundServiceFactory::GetForContext(profile)) {
    content::URLDataSource::Add(profile,
                                std::make_unique<NTPCustomImagesSource>(
                                    ntp_custom_background_images_service));
  }
}

HnsNewTabUI::~HnsNewTabUI() = default;

void HnsNewTabUI::BindInterface(
    mojo::PendingReceiver<hns_news::mojom::HnsNewsController> receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);
  // Wire up JS mojom to service
  auto* hns_news_controller =
      hns_news::HnsNewsControllerFactory::GetForContext(profile);
  if (hns_news_controller) {
    hns_news_controller->Bind(std::move(receiver));
  }
}

void HnsNewTabUI::BindInterface(
    mojo::PendingReceiver<hns_new_tab_page::mojom::PageHandlerFactory>
        pending_receiver) {
  if (page_factory_receiver_.is_bound()) {
    page_factory_receiver_.reset();
  }

  page_factory_receiver_.Bind(std::move(pending_receiver));
}

void HnsNewTabUI::CreatePageHandler(
    mojo::PendingRemote<hns_new_tab_page::mojom::Page> pending_page,
    mojo::PendingReceiver<hns_new_tab_page::mojom::PageHandler>
        pending_page_handler) {
  DCHECK(pending_page.is_valid());
  Profile* profile = Profile::FromWebUI(web_ui());
  page_handler_ = std::make_unique<HnsNewTabPageHandler>(
      std::move(pending_page_handler), std::move(pending_page), profile,
      web_ui()->GetWebContents());
}

WEB_UI_CONTROLLER_TYPE_IMPL(HnsNewTabUI)
