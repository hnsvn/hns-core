/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_

#include "hns/browser/ui/views/tabs/hns_tab_container.h"
#include "chrome/browser/ui/views/tabs/tab_container.h"
#include "chrome/browser/ui/views/tabs/tab_slot_controller.h"

class HnsTabHoverCardController;

#define UpdateHoverCard                    \
  UpdateHoverCard_Unused();                \
  friend class HnsTabHoverTest;          \
  friend class HnsTabStrip;              \
  friend class VerticalTabStripRegionView; \
  void UpdateHoverCard

#define ShouldDrawStrokes   \
  UnUsed() { return true; } \
  virtual bool ShouldDrawStrokes
#define GetDragContext                                                  \
  Unused_GetDragContext() { return nullptr; }                           \
  friend class HnsTabStrip;                                           \
  friend class HnsTabDragContext;                                     \
  static constexpr bool IsUsingHnsTabHoverCardController() {          \
    return std::is_same_v<std::unique_ptr<HnsTabHoverCardController>, \
                          decltype(TabStrip::hover_card_controller_)>;  \
  }                                                                     \
  virtual TabDragContext* GetDragContext
#define TabHoverCardController HnsTabHoverCardController
#include "src/chrome/browser/ui/views/tabs/tab_strip.h"  // IWYU pragma: export
#undef TabHoverCardController
#undef GetDragContext
#undef ShouldDrawStrokes
#undef UpdateHoverCard

static_assert(TabStrip::IsUsingHnsTabHoverCardController(),
              "Should use HnsTabHoverCardController");

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
