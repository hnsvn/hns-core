/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_HIGHLIGHT_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_HIGHLIGHT_H_

#include "chrome/browser/ui/views/tabs/tab_group_highlight.h"

class HnsTabGroupHighlight : public TabGroupHighlight {
 public:
  using TabGroupHighlight::TabGroupHighlight;
  ~HnsTabGroupHighlight() override;

 private:
  // TabGroupHighlight:
  SkPath GetPath() const override;
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_GROUP_HIGHLIGHT_H_
