/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_NEW_TAB_BUTTON_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_NEW_TAB_BUTTON_H_

#include "chrome/browser/ui/tabs/tab_style.h"

#define HNS_NEW_TAB_BUTTON_H_   \
 private:                         \
  friend class HnsNewTabButton; \
                                  \
 public:                          \
  // #define HNS_NEW_TAB_BUTTON_H_

#define PaintFill virtual PaintFill
#include "src/chrome/browser/ui/views/tabs/new_tab_button.h"  // IWYU pragma: export
#undef PaintFill
#undef HNS_NEW_TAB_BUTTON_H_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_NEW_TAB_BUTTON_H_
