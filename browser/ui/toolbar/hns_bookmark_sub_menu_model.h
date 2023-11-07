/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TOOLBAR_HNS_BOOKMARK_SUB_MENU_MODEL_H_
#define HNS_BROWSER_UI_TOOLBAR_HNS_BOOKMARK_SUB_MENU_MODEL_H_

#include <memory>

#include "hns/browser/ui/toolbar/bookmark_bar_sub_menu_model.h"
#include "chrome/browser/ui/toolbar/bookmark_sub_menu_model.h"

class Browser;

class HnsBookmarkSubMenuModel : public BookmarkSubMenuModel {
 public:
  HnsBookmarkSubMenuModel(ui::SimpleMenuModel::Delegate* delegate,
                            Browser* browser);

  HnsBookmarkSubMenuModel(const HnsBookmarkSubMenuModel&) = delete;
  HnsBookmarkSubMenuModel& operator=(const HnsBookmarkSubMenuModel&) =
      delete;

  ~HnsBookmarkSubMenuModel() override;

 private:
  void Build(Browser* browser);

  std::unique_ptr<BookmarkBarSubMenuModel> hns_bookmarks_submenu_model_;
};

#endif  // HNS_BROWSER_UI_TOOLBAR_HNS_BOOKMARK_SUB_MENU_MODEL_H_
