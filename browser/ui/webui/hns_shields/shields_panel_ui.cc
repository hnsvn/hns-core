// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/hns_shields/shields_panel_ui.h"

#include <utility>

#include "hns/browser/ui/hns_browser_window.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/hns_shields/common/hns_shield_localized_strings.h"
#include "hns/components/hns_shields/resources/panel/grit/hns_shields_panel_generated_map.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/favicon_base/favicon_url_parser.h"
#include "components/grit/hns_components_resources.h"
#include "content/public/browser/web_ui.h"
#include "net/base/features.h"

// Cache active Browser instance's TabStripModel to give
// to ShieldsPanelDataHandler when this is created because
// CreatePanelHandler() is run in async.
ShieldsPanelUI::ShieldsPanelUI(content::WebUI* web_ui)
    : ui::MojoBubbleWebUIController(web_ui, true),
      profile_(Profile::FromWebUI(web_ui)) {
  browser_ = chrome::FindLastActiveWithProfile(profile_);

  content::WebUIDataSource* source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kShieldsPanelHost);

  for (const auto& str : hns_shields::kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    source->AddString(str.name, l10n_str);
  }

  source->AddBoolean("isAdvancedViewEnabled", profile_->GetPrefs()->GetBoolean(
                                                  kShieldsAdvancedViewEnabled));

  source->AddBoolean("isHttpsByDefaultEnabled",
                     hns_shields::IsHttpsByDefaultFeatureEnabled());

  source->AddBoolean("isTorProfile", profile_->IsTor());

  source->AddBoolean("isForgetFirstPartyStorageEnabled",
                     base::FeatureList::IsEnabled(
                         net::features::kHnsForgetFirstPartyStorage));

  content::URLDataSource::Add(
      profile_, std::make_unique<FaviconSource>(
                    profile_, chrome::FaviconUrlFormat::kFavicon2));

  webui::SetupWebUIDataSource(source,
                              base::make_span(kHnsShieldsPanelGenerated,
                                              kHnsShieldsPanelGeneratedSize),
                              IDR_SHIELDS_PANEL_HTML);
}

ShieldsPanelUI::~ShieldsPanelUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(ShieldsPanelUI)

void ShieldsPanelUI::BindInterface(
    mojo::PendingReceiver<hns_shields::mojom::PanelHandlerFactory> receiver) {
  panel_factory_receiver_.reset();
  panel_factory_receiver_.Bind(std::move(receiver));
}

void ShieldsPanelUI::CreatePanelHandler(
    mojo::PendingReceiver<hns_shields::mojom::PanelHandler> panel_receiver,
    mojo::PendingReceiver<hns_shields::mojom::DataHandler>
        data_handler_receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);

  panel_handler_ = std::make_unique<ShieldsPanelHandler>(
      std::move(panel_receiver), this,
      static_cast<HnsBrowserWindow*>(browser_->window()), profile);
  data_handler_ = std::make_unique<ShieldsPanelDataHandler>(
      std::move(data_handler_receiver), this, browser_->tab_strip_model());
}
