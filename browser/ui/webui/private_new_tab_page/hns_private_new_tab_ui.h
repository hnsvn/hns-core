// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_PRIVATE_NEW_TAB_PAGE_HNS_PRIVATE_NEW_TAB_UI_H_
#define HNS_BROWSER_UI_WEBUI_PRIVATE_NEW_TAB_PAGE_HNS_PRIVATE_NEW_TAB_UI_H_

#include <memory>
#include <string>

#include "hns/components/hns_private_new_tab_ui/common/hns_private_new_tab.mojom-forward.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "ui/webui/mojo_web_ui_controller.h"

class HnsPrivateNewTabUI : public ui::MojoWebUIController {
 public:
  HnsPrivateNewTabUI(content::WebUI* web_ui, const std::string& name);
  ~HnsPrivateNewTabUI() override;
  HnsPrivateNewTabUI(const HnsPrivateNewTabUI&) = delete;
  HnsPrivateNewTabUI& operator=(const HnsPrivateNewTabUI&) = delete;

  void BindInterface(
      mojo::PendingReceiver<hns_private_new_tab::mojom::PageHandler>
          receiver);

 private:
  std::unique_ptr<hns_private_new_tab::mojom::PageHandler>
      private_tab_page_handler_;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // HNS_BROWSER_UI_WEBUI_PRIVATE_NEW_TAB_PAGE_HNS_PRIVATE_NEW_TAB_UI_H_
