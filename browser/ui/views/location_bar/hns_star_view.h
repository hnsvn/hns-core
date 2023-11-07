/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_LOCATION_BAR_HNS_STAR_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_LOCATION_BAR_HNS_STAR_VIEW_H_

#include "chrome/browser/ui/views/location_bar/star_view.h"

class HnsStarView : public StarView {
 public:
  using StarView::StarView;

  HnsStarView(const HnsStarView&) = delete;
  HnsStarView& operator=(const HnsStarView&) = delete;

 protected:
  // views::View:
  void UpdateImpl() override;
};

#endif  // HNS_BROWSER_UI_VIEWS_LOCATION_BAR_HNS_STAR_VIEW_H_
