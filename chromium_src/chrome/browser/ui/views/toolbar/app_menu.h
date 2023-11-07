/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_APP_MENU_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_APP_MENU_H_

#define RunMenu              \
  UnusedMethod1() {}         \
  friend class HnsAppMenu; \
  virtual void RunMenu

#include "src/chrome/browser/ui/views/toolbar/app_menu.h"  // IWYU pragma: export

#undef RunMenu

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_APP_MENU_H_
