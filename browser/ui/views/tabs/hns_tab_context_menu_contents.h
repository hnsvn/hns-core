/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_CONTEXT_MENU_CONTENTS_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_CONTEXT_MENU_CONTENTS_H_

#include <memory>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/ui_base_types.h"

class HnsBrowserTabStripController;
class HnsTabMenuModel;
class Browser;
class Tab;

namespace gfx {
class Point;
}  // namespace gfx

namespace sessions {
class TabRestoreService;
}  // namespace sessions

namespace views {
class MenuRunner;
}  // namespace views

class HnsTabContextMenuContents : public ui::SimpleMenuModel::Delegate {
 public:
  HnsTabContextMenuContents(Tab* tab,
                              HnsBrowserTabStripController* controller,
                              int index);
  HnsTabContextMenuContents(const HnsTabContextMenuContents&) = delete;
  HnsTabContextMenuContents& operator=(const HnsTabContextMenuContents&) =
      delete;
  ~HnsTabContextMenuContents() override;

  void Cancel();

  void RunMenuAt(const gfx::Point& point, ui::MenuSourceType source_type);

  // ui::SimpleMenuModel::Delegate overrides:
  bool IsCommandIdChecked(int command_id) const override;
  bool IsCommandIdEnabled(int command_id) const override;
  bool IsCommandIdVisible(int command_id) const override;
  bool GetAcceleratorForCommandId(int command_id,
                                  ui::Accelerator* accelerator) const override;
  void ExecuteCommand(int command_id, int event_flags) override;

 private:
  FRIEND_TEST_ALL_PREFIXES(VerticalTabStripStringBrowserTest,
                           ContextMenuString);

  bool IsHnsCommandIdEnabled(int command_id) const;
  void ExecuteHnsCommand(int command_id);
  bool IsHnsCommandId(int command_id) const;

  std::unique_ptr<HnsTabMenuModel> model_;
  std::unique_ptr<views::MenuRunner> menu_runner_;

  raw_ptr<Tab> tab_ = nullptr;
  int tab_index_;
  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<sessions::TabRestoreService> restore_service_ = nullptr;
  raw_ptr<HnsBrowserTabStripController> controller_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_CONTEXT_MENU_CONTENTS_H_
