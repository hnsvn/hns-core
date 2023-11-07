/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_WELCOME_PAGE_HNS_WELCOME_UI_H_
#define HNS_BROWSER_UI_WEBUI_WELCOME_PAGE_HNS_WELCOME_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class HnsWelcomeUI : public content::WebUIController {
 public:
  HnsWelcomeUI(content::WebUI* web_ui, const std::string& host);
  ~HnsWelcomeUI() override;
  HnsWelcomeUI(const HnsWelcomeUI&) = delete;
  HnsWelcomeUI& operator=(const HnsWelcomeUI&) = delete;
};

#endif  // HNS_BROWSER_UI_WEBUI_WELCOME_PAGE_HNS_WELCOME_UI_H_
