// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_ADBLOCK_INTERNALS_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_ADBLOCK_INTERNALS_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

// The WebUI for hns://adblock-internals
class HnsAdblockInternalsUI : public content::WebUIController {
 public:
  HnsAdblockInternalsUI(content::WebUI* web_ui, const std::string& name);

  HnsAdblockInternalsUI(const HnsAdblockInternalsUI&) = delete;
  HnsAdblockInternalsUI& operator=(const HnsAdblockInternalsUI&) = delete;

  ~HnsAdblockInternalsUI() override;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_ADBLOCK_INTERNALS_UI_H_
