/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_

#include "hns/browser/ui/toolbar/hns_bookmark_context_menu_controller.h"

#define BookmarkContextMenuController HnsBookmarkContextMenuController
#define close_on_remove_ \
  close_on_remove_;      \
  friend class HnsBookmarkContextMenu
#include "src/chrome/browser/ui/views/bookmarks/bookmark_context_menu.h"  // IWYU pragma: export
#undef BookmarkContextMenuController
#undef close_on_remove_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_
