/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/bookmark_bar_sub_menu_model.h"

#include "hns/browser/ui/bookmark/bookmark_helper.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/grit/generated_resources.h"
#include "components/prefs/pref_service.h"
#include "ui/base/models/simple_menu_model.h"

BookmarkBarSubMenuModel::BookmarkBarSubMenuModel(Profile* profile)
    : SimpleMenuModel(this), profile_(profile) {
  Build();
}

BookmarkBarSubMenuModel::~BookmarkBarSubMenuModel() = default;

void BookmarkBarSubMenuModel::Build() {
  AddCheckItemWithStringId(IDC_HNS_BOOKMARK_BAR_ALWAYS,
                           IDS_BOOKMAR_BAR_MENU_SHOW_ALWAYS);

  AddCheckItemWithStringId(IDC_HNS_BOOKMARK_BAR_NEVER,
                           IDS_BOOKMAR_BAR_MENU_SHOW_NEVER);

  AddCheckItemWithStringId(IDC_HNS_BOOKMARK_BAR_NTP,
                           IDS_BOOKMAR_BAR_MENU_SHOW_NTP);
}

void BookmarkBarSubMenuModel::ExecuteCommand(int command_id, int event_flags) {
  switch (command_id) {
    case IDC_HNS_BOOKMARK_BAR_ALWAYS:
      hns::SetBookmarkState(hns::BookmarkBarState::kAlways,
                              profile_->GetPrefs());
      return;
    case IDC_HNS_BOOKMARK_BAR_NEVER:
      hns::SetBookmarkState(hns::BookmarkBarState::kNever,
                              profile_->GetPrefs());
      return;
    case IDC_HNS_BOOKMARK_BAR_NTP:
      hns::SetBookmarkState(hns::BookmarkBarState::kNtp,
                              profile_->GetPrefs());
      return;
  }
}

bool BookmarkBarSubMenuModel::IsCommandIdChecked(int command_id) const {
  switch (hns::GetBookmarkBarState(profile_->GetPrefs())) {
    case hns::BookmarkBarState::kAlways:
      return command_id == IDC_HNS_BOOKMARK_BAR_ALWAYS;
    case hns::BookmarkBarState::kNtp:
      return command_id == IDC_HNS_BOOKMARK_BAR_NTP;
    case hns::BookmarkBarState::kNever:
      return command_id == IDC_HNS_BOOKMARK_BAR_NEVER;
  }
  return false;
}

bool BookmarkBarSubMenuModel::IsCommandIdEnabled(int command_id) const {
  return (command_id == IDC_HNS_BOOKMARK_BAR_ALWAYS ||
          command_id == IDC_HNS_BOOKMARK_BAR_NEVER ||
          command_id == IDC_HNS_BOOKMARK_BAR_NTP);
}
