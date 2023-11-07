// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_NEWS_INTERNALS_HNS_NEWS_INTERNALS_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_NEWS_INTERNALS_HNS_NEWS_INTERNALS_UI_H_

#include <memory>
#include <string>

#include "hns/components/hns_news/common/hns_news.mojom-forward.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

class HnsNewsInternalsUI : public content::WebUIController {
 public:
  explicit HnsNewsInternalsUI(content::WebUI* web_ui,
                                const std::string& host);
  HnsNewsInternalsUI(const HnsNewsInternalsUI&) = delete;
  HnsNewsInternalsUI& operator=(const HnsNewsInternalsUI&) = delete;
  ~HnsNewsInternalsUI() override;

  void BindInterface(
      mojo::PendingReceiver<hns_news::mojom::HnsNewsController> receiver);

 private:
  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_NEWS_INTERNALS_HNS_NEWS_INTERNALS_UI_H_
