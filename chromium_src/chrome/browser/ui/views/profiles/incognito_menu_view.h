/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_INCOGNITO_MENU_VIEW_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_INCOGNITO_MENU_VIEW_H_

#define HNS_INCOGNITO_MENU_VIEW_H_   \
 private:                              \
  friend class HnsIncognitoMenuView; \
                                       \
 public:
#define OnExitButtonClicked virtual OnExitButtonClicked
#include "src/chrome/browser/ui/views/profiles/incognito_menu_view.h"  // IWYU pragma: export
#undef OnExitButtonClicked
#undef HNS_INCOGNITO_MENU_VIEW_H_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_INCOGNITO_MENU_VIEW_H_
