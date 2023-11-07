/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_STRIP_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_STRIP_H_

#include <memory>

#include "base/gtest_prod_util.h"
#include "chrome/browser/ui/views/tabs/tab_strip.h"

class Tab;
class HnsTabStrip : public TabStrip {
 public:
  METADATA_HEADER(HnsTabStrip);

  explicit HnsTabStrip(std::unique_ptr<TabStripController> controller);
  ~HnsTabStrip() override;
  HnsTabStrip(const HnsTabStrip&) = delete;
  HnsTabStrip& operator=(const HnsTabStrip&) = delete;

  bool IsVerticalTabsFloating() const;

  // TabStrip:
  void UpdateHoverCard(Tab* tab, HoverCardUpdateType update_type) override;
  void MaybeStartDrag(
      TabSlotView* source,
      const ui::LocatedEvent& event,
      const ui::ListSelectionModel& original_selection) override;
  void AddedToWidget() override;
  absl::optional<int> GetCustomBackgroundId(
      BrowserFrameActiveState active_state) const override;

 private:
  FRIEND_TEST_ALL_PREFIXES(ColorPaletteTest, LightThemeMinimumContrast);

  void UpdateTabContainer();
  bool ShouldShowVerticalTabs() const;

  // TabStrip overrides:
  SkColor GetTabSeparatorColor() const override;
  bool ShouldDrawStrokes() const override;
  int GetStrokeThickness() const override;
  void Layout() override;

  // Exposed for testing.
  static constexpr float kHnsMinimumContrastRatioForOutlines = 1.2797f;
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_STRIP_H_
