/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_BOOKMARKS_HNS_BOOKMARK_CONTEXT_MENU_H_
#define HNS_BROWSER_UI_VIEWS_BOOKMARKS_HNS_BOOKMARK_CONTEXT_MENU_H_

#include <vector>

#include "chrome/browser/ui/views/bookmarks/bookmark_context_menu.h"

class HnsBookmarkContextMenu : public BookmarkContextMenu {
 public:
  // |browser| is used to open bookmarks as well as the bookmark manager, and
  // is NULL in tests.
  HnsBookmarkContextMenu(
      views::Widget* parent_widget,
      Browser* browser,
      Profile* profile,
      BookmarkLaunchLocation opened_from,
      const bookmarks::BookmarkNode* parent,
      const std::vector<const bookmarks::BookmarkNode*>& selection,
      bool close_on_remove);

  HnsBookmarkContextMenu(const HnsBookmarkContextMenu&) = delete;
  HnsBookmarkContextMenu& operator=(const HnsBookmarkContextMenu&) = delete;

  ~HnsBookmarkContextMenu() override;
};

#endif  // HNS_BROWSER_UI_VIEWS_BOOKMARKS_HNS_BOOKMARK_CONTEXT_MENU_H_
