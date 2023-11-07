/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TAB_SEARCH_BUBBLE_HOST_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TAB_SEARCH_BUBBLE_HOST_H_

#define ShowTabSearchBubble              \
  ShowTabSearchBubble_Unused();          \
  friend class HnsTabSearchBubbleHost; \
  virtual bool ShowTabSearchBubble

#include "src/chrome/browser/ui/views/tab_search_bubble_host.h"  // IWYU pragma: export

#undef ShowTabSearchBubble

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TAB_SEARCH_BUBBLE_HOST_H_
