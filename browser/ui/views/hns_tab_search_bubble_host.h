/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_TAB_SEARCH_BUBBLE_HOST_H_
#define HNS_BROWSER_UI_VIEWS_HNS_TAB_SEARCH_BUBBLE_HOST_H_

#include "chrome/browser/ui/views/tab_search_bubble_host.h"

class HnsTabSearchBubbleHost : public TabSearchBubbleHost {
 public:
  using TabSearchBubbleHost::TabSearchBubbleHost;
  ~HnsTabSearchBubbleHost() override = default;

  void SetBubbleArrow(views::BubbleBorder::Arrow arrow);

  // TabSearchBubbleHost:
  bool ShowTabSearchBubble(
      bool triggered_by_keyboard_shortcut = false) override;

 private:
  absl::optional<views::BubbleBorder::Arrow> arrow_;
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_TAB_SEARCH_BUBBLE_HOST_H_
