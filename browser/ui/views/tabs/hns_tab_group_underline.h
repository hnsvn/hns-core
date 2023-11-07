/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_UNDERLINE_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_UNDERLINE_H_

#include "chrome/browser/ui/views/tabs/tab_group_underline.h"

// In vertical tabs, TabGroupUnderline is not actually "underline'. It's an
// enclosing rounded rect for views in the group.
class HnsTabGroupUnderline : public TabGroupUnderline {
 public:
  HnsTabGroupUnderline(TabGroupViews* tab_group_views,
                         const tab_groups::TabGroupId& group,
                         const TabGroupStyle& style);
  ~HnsTabGroupUnderline() override;

  // TabGroupUnderline:
  void UpdateBounds(const views::View* leading_view,
                    const views::View* trailing_view) override;
  gfx::Insets GetInsetsForUnderline(
      const views::View* sibling_view) const override;
  gfx::Rect CalculateTabGroupUnderlineBounds(
      const views::View* underline_view,
      const views::View* leading_view,
      const views::View* trailing_view) const override;

 private:
  bool ShouldShowVerticalTabs() const;
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_UNDERLINE_H_
