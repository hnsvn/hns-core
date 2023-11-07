// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/hns_news_internals/hns_news_internals_ui.h"

#include <string>
#include <utility>

#include "hns/browser/hns_news/hns_news_controller_factory.h"
#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "hns/components/hns_news/browser/resources/grit/hns_news_internals_generated_map.h"
#include "chrome/browser/profiles/profile.h"
#include "components/grit/hns_components_resources.h"

HnsNewsInternalsUI::HnsNewsInternalsUI(content::WebUI* web_ui,
                                           const std::string& host)
    : content::WebUIController(web_ui) {
  auto* source = CreateAndAddWebUIDataSource(
      web_ui, host, kHnsNewsInternalsGenerated,
      kHnsNewsInternalsGeneratedSize, IDR_HNS_NEWS_INTERNALS_HTML);
  DCHECK(source);
}

HnsNewsInternalsUI::~HnsNewsInternalsUI() = default;
WEB_UI_CONTROLLER_TYPE_IMPL(HnsNewsInternalsUI)

void HnsNewsInternalsUI::BindInterface(
    mojo::PendingReceiver<hns_news::mojom::HnsNewsController> receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  auto* controller =
      hns_news::HnsNewsControllerFactory::GetForContext(profile);
  if (!controller) {
    return;
  }

  controller->Bind(std::move(receiver));
}
