/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_GROUP_HEADER_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_GROUP_HEADER_H_

#define TabGroupEditorBubbleViewDialogBrowserTest \
  TabGroupEditorBubbleViewDialogBrowserTest;      \
  friend class HnsTabGroupHeader
#define VisualsChanged virtual VisualsChanged

#include "src/chrome/browser/ui/views/tabs/tab_group_header.h"  // IWYU pragma: export

#undef VisualsChanged
#undef TabGroupEditorBubbleViewDialogBrowserTest

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_GROUP_HEADER_H_
