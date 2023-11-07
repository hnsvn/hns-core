/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_SIDEBAR_SIDEBAR_H_
#define HNS_BROWSER_UI_SIDEBAR_SIDEBAR_H_

#include "hns/components/sidebar/sidebar_service.h"

namespace sidebar {

// Interact with UI layer.
class Sidebar {
 public:
  virtual void SetSidebarShowOption(
      SidebarService::ShowSidebarOption show_option) = 0;

  // Update sidebar item's UI state.
  virtual void UpdateSidebarItemsState() = 0;

  // Return true if active tab(web contents) has associated
  // side panel.
  virtual bool HasActiveContextualEntry() = 0;

 protected:
  virtual ~Sidebar() {}
};

}  // namespace sidebar

#endif  // HNS_BROWSER_UI_SIDEBAR_SIDEBAR_H_
