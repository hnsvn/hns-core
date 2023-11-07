/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_COOKIE_LIST_OPT_IN_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_COOKIE_LIST_OPT_IN_UI_H_

#include <memory>

#include "hns/components/hns_shields/common/cookie_list_opt_in.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

class CookieListOptInUI
    : public ui::MojoBubbleWebUIController,
      public hns_shields::mojom::CookieListOptInPageHandlerFactory {
 public:
  explicit CookieListOptInUI(content::WebUI* web_ui);
  ~CookieListOptInUI() override;

  CookieListOptInUI(const CookieListOptInUI&) = delete;
  CookieListOptInUI& operator=(const CookieListOptInUI&) = delete;

  void BindInterface(
      mojo::PendingReceiver<CookieListOptInPageHandlerFactory> reciever);

 private:
  // hns_shields::mojom::CookieListOptInPageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingReceiver<hns_shields::mojom::CookieListOptInPageHandler>
          receiver) override;

  std::unique_ptr<hns_shields::mojom::CookieListOptInPageHandler>
      page_handler_;
  mojo::Receiver<CookieListOptInPageHandlerFactory> page_factory_receiver_{
      this};

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_COOKIE_LIST_OPT_IN_UI_H_
