// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/speedreader/speedreader_toolbar_ui.h"

#include <utility>

#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/components/speedreader/common/constants.h"
#include "hns/components/speedreader/resources/panel/grit/hns_speedreader_toolbar_generated_map.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "components/grit/hns_components_resources.h"
#include "content/public/browser/host_zoom_map.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/url_constants.h"

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/components/ai_chat/common/features.h"
#endif

SpeedreaderToolbarUI::SpeedreaderToolbarUI(content::WebUI* web_ui,
                                           const std::string& name)
    : ui::MojoBubbleWebUIController(web_ui, true),
      profile_(Profile::FromWebUI(web_ui)) {
  content::HostZoomMap::Get(web_ui->GetWebContents()->GetSiteInstance())
      ->SetZoomLevelForHostAndScheme(content::kChromeUIScheme,
                                     kSpeedreaderPanelHost, 0);

  browser_ = chrome::FindLastActiveWithProfile(profile_);

  content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
      web_ui, name, kHnsSpeedreaderToolbarGenerated,
      kHnsSpeedreaderToolbarGeneratedSize, IDR_SPEEDREADER_UI_HTML);

  for (const auto& str : speedreader::kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    source->AddString(str.name, l10n_str);
  }

#if BUILDFLAG(ENABLE_AI_CHAT)
  source->AddBoolean("aiChatFeatureEnabled",
                     ai_chat::features::IsAIChatEnabled());
#else
  source->AddBoolean("aiChatFeatureEnabled", false);
#endif
}

SpeedreaderToolbarUI::~SpeedreaderToolbarUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(SpeedreaderToolbarUI)

void SpeedreaderToolbarUI::BindInterface(
    mojo::PendingReceiver<speedreader::mojom::ToolbarFactory> receiver) {
  toolbar_factory_.reset();
  toolbar_factory_.Bind(std::move(receiver));
}

void SpeedreaderToolbarUI::CreateInterfaces(
    mojo::PendingReceiver<speedreader::mojom::ToolbarDataHandler>
        toolbar_data_handler,
    mojo::PendingRemote<speedreader::mojom::ToolbarEventsHandler>
        toolbar_events_handler) {
  toolbar_data_handler_ = std::make_unique<SpeedreaderToolbarDataHandlerImpl>(
      browser_, std::move(toolbar_data_handler),
      std::move(toolbar_events_handler));
}
