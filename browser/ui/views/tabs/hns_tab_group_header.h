/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_HEADER_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_HEADER_H_

#include "chrome/browser/ui/views/tabs/tab_group_header.h"

namespace tab_groups {
class TabGroupId;
}  // namespace tab_groups

class HnsTabGroupHeader : public TabGroupHeader {
 public:
  METADATA_HEADER(HnsTabGroupHeader);

  constexpr static int kPaddingForGroup = 4;

  using TabGroupHeader::TabGroupHeader;
  ~HnsTabGroupHeader() override;

  // TabGroupHeader:
  void AddedToWidget() override;
  void VisualsChanged() override;
  void Layout() override;

 private:
  bool ShouldShowVerticalTabs() const;
  void LayoutTitleChip();
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_HEADER_H_
