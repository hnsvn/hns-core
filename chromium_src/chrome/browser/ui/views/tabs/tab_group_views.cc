/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/tabs/hns_tab_group_header.h"
#include "hns/browser/ui/views/tabs/hns_tab_group_highlight.h"
#include "hns/browser/ui/views/tabs/hns_tab_group_underline.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/ui/views/tabs/tab_group_style.h"
#include "ui/views/view_utils.h"

#define TabGroupHeader HnsTabGroupHeader
#define TabGroupUnderline HnsTabGroupUnderline
#define TabGroupHighlight HnsTabGroupHighlight

// TabGroupViews destructor is not virtual, so we can't override the method.
#define HNS_TAB_GROUP_VIEWS_GET_LEADING_TRAILING_GROUP_VIEWS                 \
  if (base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs) &&      \
      tabs::utils::ShouldShowVerticalTabs(                                     \
          tab_slot_controller_->GetBrowser())) {                               \
    std::vector<views::View*> children_in_same_group;                          \
    base::ranges::copy_if(                                                     \
        children, std::back_inserter(children_in_same_group),                  \
        [this](auto* child) {                                                  \
          TabSlotView* tab_slot_view = views::AsViewClass<TabSlotView>(child); \
          return tab_slot_view && tab_slot_view->group() == group_ &&          \
                 tab_slot_view->GetVisible();                                  \
        });                                                                    \
    if (children_in_same_group.empty()) {                                      \
      return {nullptr, nullptr};                                               \
    }                                                                          \
    return base::ranges::minmax(children_in_same_group,                        \
                                [](const auto* a, const auto* b) {             \
                                  return a->GetBoundsInScreen().bottom() <     \
                                         b->GetBoundsInScreen().bottom();      \
                                });                                            \
  }

#include "src/chrome/browser/ui/views/tabs/tab_group_views.cc"

#undef HNS_TAB_GROUP_VIEWS_GET_LEADING_TRAILING_GROUP_VIEWS
#undef TabGroupHighlight
#undef TabGroupUnderline
#undef TabGroupHeader

const Browser* TabGroupViews::GetBrowser() const {
  return tab_slot_controller_->GetBrowser();
}
