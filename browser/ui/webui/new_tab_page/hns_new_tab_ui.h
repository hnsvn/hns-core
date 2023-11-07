// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_UI_H_
#define HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_UI_H_

#include <memory>
#include <string>

#include "hns/components/hns_new_tab_ui/hns_new_tab_page.mojom.h"
#include "hns/components/hns_news/common/hns_news.mojom.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_web_ui_controller.h"

namespace hns_news {
class HnsNewsController;
}  // namespace hns_news

class HnsNewTabPageHandler;

class HnsNewTabUI : public ui::MojoWebUIController,
                      public hns_new_tab_page::mojom::PageHandlerFactory {
 public:
  HnsNewTabUI(content::WebUI* web_ui, const std::string& name);
  ~HnsNewTabUI() override;
  HnsNewTabUI(const HnsNewTabUI&) = delete;
  HnsNewTabUI& operator=(const HnsNewTabUI&) = delete;

  // Instantiates the implementor of the mojo
  // interface passing the pending receiver that will be internally bound.
  void BindInterface(
      mojo::PendingReceiver<hns_news::mojom::HnsNewsController> receiver);

  void BindInterface(
      mojo::PendingReceiver<hns_new_tab_page::mojom::PageHandlerFactory>
          pending_receiver);

 private:
  // new_tab_page::mojom::PageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingRemote<hns_new_tab_page::mojom::Page> pending_page,
      mojo::PendingReceiver<hns_new_tab_page::mojom::PageHandler>
          pending_page_handler) override;

  std::unique_ptr<HnsNewTabPageHandler> page_handler_;
  mojo::Receiver<hns_new_tab_page::mojom::PageHandlerFactory>
      page_factory_receiver_;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_UI_H_
