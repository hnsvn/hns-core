/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/hns_bookmark_context_menu_controller.h"

#include <memory>
#include <vector>

#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/ui/bookmarks/bookmark_context_menu_controller.h"
#include "chrome/grit/generated_resources.h"

HnsBookmarkContextMenuController::HnsBookmarkContextMenuController(
    gfx::NativeWindow parent_window,
    BookmarkContextMenuControllerDelegate* delegate,
    Browser* browser,
    Profile* profile,
    BookmarkLaunchLocation opened_from,
    const bookmarks::BookmarkNode* parent,
    const std::vector<const bookmarks::BookmarkNode*>& selection)
    : BookmarkContextMenuController(parent_window,
                                    delegate,
                                    browser,
                                    profile,
                                    opened_from,
                                    parent,
                                    selection) {
  AddHnsBookmarksSubmenu(profile);
}

HnsBookmarkContextMenuController::~HnsBookmarkContextMenuController() =
    default;

void HnsBookmarkContextMenuController::AddHnsBookmarksSubmenu(
    Profile* profile) {
  auto index = menu_model()->GetIndexOfCommandId(IDC_BOOKMARK_BAR_ALWAYS_SHOW);
  if (!index.has_value())
    return;
  menu_model()->RemoveItemAt(index.value());
  hns_bookmarks_submenu_model_ =
      std::make_unique<BookmarkBarSubMenuModel>(profile);

  menu_model()->InsertSubMenuWithStringIdAt(
      index.value(), IDC_HNS_BOOKMARK_BAR_SUBMENU, IDS_SHOW_BOOKMARK_BAR,
      hns_bookmarks_submenu_model_.get());
}

bool HnsBookmarkContextMenuController::IsCommandIdChecked(
    int command_id) const {
  if (hns_bookmarks_submenu_model_->GetIndexOfCommandId(command_id))
    return hns_bookmarks_submenu_model_->IsCommandIdChecked(command_id);
  return BookmarkContextMenuController::IsCommandIdChecked(command_id);
}

bool HnsBookmarkContextMenuController::IsCommandIdEnabled(
    int command_id) const {
  if (hns_bookmarks_submenu_model_->GetIndexOfCommandId(command_id))
    return hns_bookmarks_submenu_model_->IsCommandIdEnabled(command_id);
  return BookmarkContextMenuController::IsCommandIdEnabled(command_id);
}

bool HnsBookmarkContextMenuController::IsCommandIdVisible(
    int command_id) const {
  if (hns_bookmarks_submenu_model_->GetIndexOfCommandId(command_id))
    return hns_bookmarks_submenu_model_->IsCommandIdVisible(command_id);
  return BookmarkContextMenuController::IsCommandIdVisible(command_id);
}

void HnsBookmarkContextMenuController::ExecuteCommand(int command_id,
                                                        int event_flags) {
  if (hns_bookmarks_submenu_model_->GetIndexOfCommandId(command_id)) {
    hns_bookmarks_submenu_model_->ExecuteCommand(command_id, event_flags);
    return;
  }
  BookmarkContextMenuController::ExecuteCommand(command_id, event_flags);
}

std::u16string HnsBookmarkContextMenuController::GetLabelForCommandId(
    int command_id) const {
  if (hns_bookmarks_submenu_model_->GetIndexOfCommandId(command_id))
    return hns_bookmarks_submenu_model_->GetLabelForCommandId(command_id);
  return BookmarkContextMenuController::GetLabelForCommandId(command_id);
}

BookmarkBarSubMenuModel*
HnsBookmarkContextMenuController::GetBookmarkSubmenuModel() {
  return hns_bookmarks_submenu_model_.get();
}
