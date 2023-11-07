/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_

#define HNS_LOCATION_BAR_VIEW_H_   \
 private:                            \
  friend class HnsLocationBarView; \
                                     \
 public:                             \
  virtual std::vector<views::View*> GetTrailingViews();

#define OnOmniboxBlurred virtual OnOmniboxBlurred
#define GetBorderRadius virtual GetBorderRadius
#include "src/chrome/browser/ui/views/location_bar/location_bar_view.h"  // IWYU pragma: export
#undef GetBorderRadius
#undef OnOmniboxBlurred
#undef HNS_LOCATION_BAR_VIEW_H_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
