/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_BROWSER_TAB_STRIP_CONTROLLER_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_BROWSER_TAB_STRIP_CONTROLLER_H_

#include <memory>

#include "chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"

class HnsTabContextMenuContents;

class HnsBrowserTabStripController : public BrowserTabStripController {
 public:
  HnsBrowserTabStripController(TabStripModel* model,
                                 BrowserView* browser_view,
                                 std::unique_ptr<TabMenuModelFactory>
                                     menu_model_factory_override = nullptr);
  HnsBrowserTabStripController(const HnsBrowserTabStripController&) =
      delete;
  HnsBrowserTabStripController& operator=(
      const HnsBrowserTabStripController&) = delete;
  ~HnsBrowserTabStripController() override;

  // BrowserTabStripController overrides:
  void ShowContextMenuForTab(Tab* tab,
                             const gfx::Point& p,
                             ui::MenuSourceType source_type) override;

 private:
  // If non-NULL it means we're showing a menu for the tab.
  std::unique_ptr<HnsTabContextMenuContents> context_menu_contents_;
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_BROWSER_TAB_STRIP_CONTROLLER_H_
