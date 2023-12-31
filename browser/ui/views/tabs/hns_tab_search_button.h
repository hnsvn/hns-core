/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_SEARCH_BUTTON_H_
#define HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_SEARCH_BUTTON_H_

#include "chrome/browser/ui/views/tabs/tab_search_button.h"
#include "third_party/skia/include/core/SkPath.h"
#include "ui/gfx/geometry/size.h"

class HnsTabSearchButton : public TabSearchButton {
 public:
  METADATA_HEADER(HnsTabSearchButton);

  explicit HnsTabSearchButton(TabStrip* tab_strip);
  ~HnsTabSearchButton() override;
  HnsTabSearchButton(const HnsTabSearchButton&) = delete;
  HnsTabSearchButton& operator=(const HnsTabSearchButton&) = delete;

  void SetBubbleArrow(views::BubbleBorder::Arrow arrow);

  // TabSearchButton overrides:
  gfx::Size CalculatePreferredSize() const override;
  int GetCornerRadius() const override;
};

#endif  // HNS_BROWSER_UI_VIEWS_TABS_HNS_TAB_SEARCH_BUTTON_H_
