/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_COOKIE_LIST_OPT_IN_PAGE_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_COOKIE_LIST_OPT_IN_PAGE_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_shields/common/cookie_list_opt_in.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

class Profile;

class CookieListOptInPageHandler
    : public hns_shields::mojom::CookieListOptInPageHandler {
 public:
  CookieListOptInPageHandler(
      mojo::PendingReceiver<hns_shields::mojom::CookieListOptInPageHandler>
          receiver,
      base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder,
      Profile* profile);

  CookieListOptInPageHandler(const CookieListOptInPageHandler&) = delete;
  CookieListOptInPageHandler& operator=(const CookieListOptInPageHandler&) =
      delete;

  ~CookieListOptInPageHandler() override;

  // hns_shields::mojom::CookieListOptInPageHandler:
  void ShowUI() override;
  void CloseUI() override;
  void EnableFilter() override;

  void OnUINoClicked() override;
  void OnUIYesClicked() override;

 private:
  mojo::Receiver<hns_shields::mojom::CookieListOptInPageHandler> receiver_;
  base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder_;
  raw_ptr<Profile> profile_ = nullptr;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_COOKIE_LIST_OPT_IN_PAGE_HANDLER_H_
