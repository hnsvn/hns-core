/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_SEARCH_ENGINES_HANDLER_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_SEARCH_ENGINES_HANDLER_H_

#define GetSearchEnginesList                    \
  GetSearchEnginesList_NotUsed() { return {}; } \
  friend class HnsSearchEnginesHandler;       \
  virtual base::Value::Dict GetSearchEnginesList

#include "src/chrome/browser/ui/webui/settings/search_engines_handler.h"  // IWYU pragma: export

#undef GetSearchEnginesList

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_SEARCH_ENGINES_HANDLER_H_
