/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TABS_HNS_TAB_MENU_MODEL_H_
#define HNS_BROWSER_UI_TABS_HNS_TAB_MENU_MODEL_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/tabs/tab_menu_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"

namespace content {
class WebContents;
}  // namespace content

namespace sessions {
class TabRestoreService;
}  // namespace sessions

class HnsTabMenuModel : public TabMenuModel {
 public:
  enum HnsTabContextMenuCommand {
    CommandStart = TabStripModel::CommandLast,
    CommandRestoreTab,
    CommandBookmarkAllTabs,
    CommandShowVerticalTabs,
    CommandToggleTabMuted,
    CommandLast,
  };

  HnsTabMenuModel(ui::SimpleMenuModel::Delegate* delegate,
                    TabMenuModelDelegate* tab_menu_model_delegate,
                    TabStripModel* tab_strip_model,
                    int index,
                    bool is_vertical_tab);
  HnsTabMenuModel(const HnsTabMenuModel&) = delete;
  HnsTabMenuModel& operator=(const HnsTabMenuModel&) = delete;
  ~HnsTabMenuModel() override;

  bool all_muted() const { return all_muted_; }

  // TabMenuModel:
  std::u16string GetLabelAt(size_t index) const override;

 private:
  void Build(int selected_tab_count);
  int GetRestoreTabCommandStringId() const;

  raw_ptr<content::WebContents> web_contents_ = nullptr;
  raw_ptr<sessions::TabRestoreService> restore_service_ = nullptr;
  bool all_muted_;

  bool is_vertical_tab_ = false;
};

#endif  // HNS_BROWSER_UI_TABS_HNS_TAB_MENU_MODEL_H_
