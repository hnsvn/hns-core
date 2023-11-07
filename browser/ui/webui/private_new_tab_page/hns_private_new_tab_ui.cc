// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/private_new_tab_page/hns_private_new_tab_ui.h"

#include <utility>

#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/browser/ui/webui/private_new_tab_page/hns_private_new_tab_page_handler.h"
#include "hns/components/hns_private_new_tab/resources/page/grit/hns_private_new_tab_generated_map.h"
#include "hns/components/hns_private_new_tab_ui/common/constants.h"
#include "hns/components/hns_private_new_tab_ui/common/pref_names.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/profiles/profile.h"
#include "components/grit/hns_components_resources.h"
#include "components/prefs/pref_service.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/web_ui_data_source.h"

HnsPrivateNewTabUI::HnsPrivateNewTabUI(content::WebUI* web_ui,
                                           const std::string& name)
    : ui::MojoWebUIController(web_ui, false) {
  Profile* profile = Profile::FromWebUI(web_ui);

  web_ui->OverrideTitle(
      hns_l10n::GetLocalizedResourceUTF16String(IDS_NEW_INCOGNITO_TAB_TITLE));

  content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
      web_ui, name, kHnsPrivateNewTabGenerated,
      kHnsPrivateNewTabGeneratedSize, IDR_HNS_PRIVATE_NEW_TAB_HTML);

  for (const auto& str : hns_private_new_tab::kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    source->AddString(str.name, l10n_str);
  }

  source->AddBoolean("isWindowTor", profile->IsTor());

  AddBackgroundColorToSource(source, web_ui->GetWebContents());
}

HnsPrivateNewTabUI::~HnsPrivateNewTabUI() = default;

void HnsPrivateNewTabUI::BindInterface(
    mojo::PendingReceiver<hns_private_new_tab::mojom::PageHandler> receiver) {
  private_tab_page_handler_ = std::make_unique<HnsPrivateNewTabPageHandler>(
      Profile::FromWebUI(web_ui()), web_ui()->GetWebContents(),
      std::move(receiver));
}

WEB_UI_CONTROLLER_TYPE_IMPL(HnsPrivateNewTabUI)
