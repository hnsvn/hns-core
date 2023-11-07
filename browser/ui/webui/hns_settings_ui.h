/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_SETTINGS_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_SETTINGS_UI_H_

#include <memory>
#include <string>

#include "hns/components/commands/common/commands.mojom.h"
#include "chrome/browser/ui/webui/settings/settings_ui.h"

namespace content {
class WebUIDataSource;
}

class Profile;

class HnsSettingsUI : public settings::SettingsUI {
 public:
  HnsSettingsUI(content::WebUI* web_ui, const std::string& host);
  HnsSettingsUI(const HnsSettingsUI&) = delete;
  HnsSettingsUI& operator=(const HnsSettingsUI&) = delete;
  ~HnsSettingsUI() override;

  static void AddResources(content::WebUIDataSource* html_source,
                           Profile* profile);
  // Allows disabling CSP on settings page so EvalJS could be run in main world.
  static bool& ShouldDisableCSPForTesting();

  static bool& ShouldExposeElementsForTesting();

  void BindInterface(
      mojo::PendingReceiver<commands::mojom::CommandsService> pending_receiver);
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_SETTINGS_UI_H_
