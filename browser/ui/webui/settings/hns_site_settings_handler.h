/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SITE_SETTINGS_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SITE_SETTINGS_HANDLER_H_

#include <string>

#include "chrome/browser/ui/webui/settings/site_settings_handler.h"

namespace settings {

class HnsSiteSettingsHandler : public SiteSettingsHandler {
 public:
  explicit HnsSiteSettingsHandler(Profile* profile);

  HnsSiteSettingsHandler(const HnsSiteSettingsHandler&) = delete;
  HnsSiteSettingsHandler& operator=(const HnsSiteSettingsHandler&) = delete;

  ~HnsSiteSettingsHandler() override;

  // SettingsPageUIHandler:
  void RegisterMessages() override;

  // Returns whether the pattern is valid given the type.
  void HandleIsPatternValidForType(const base::Value::List& args);

  bool IsPatternValidForHnsContentType(ContentSettingsType content_type,
                                         const std::string& pattern_string);

 private:
  friend class TestHnsSiteSettingsHandlerUnittest;
};

}  // namespace settings

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SITE_SETTINGS_HANDLER_H_
