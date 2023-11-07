/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_REWARDS_PAGE_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_REWARDS_PAGE_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class HnsRewardsPageUI : public content::WebUIController {
 public:
  HnsRewardsPageUI(content::WebUI* web_ui, const std::string& host);
  ~HnsRewardsPageUI() override;
  HnsRewardsPageUI(const HnsRewardsPageUI&) = delete;
  HnsRewardsPageUI& operator=(const HnsRewardsPageUI&) = delete;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_REWARDS_PAGE_UI_H_
