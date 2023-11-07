/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_OMNIBOX_POPUP_VIEW_VIEWS_H_
#define HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_OMNIBOX_POPUP_VIEW_VIEWS_H_

#include "chrome/browser/ui/views/omnibox/omnibox_popup_view_views.h"

class HnsOmniboxPopupViewViews : public OmniboxPopupViewViews {
 public:
  METADATA_HEADER(HnsOmniboxPopupViewViews);

  using OmniboxPopupViewViews::OmniboxPopupViewViews;
  ~HnsOmniboxPopupViewViews() override;

  // OmniboxPopupViewViews:
  gfx::Rect GetTargetBounds() const override;
};

#endif  // HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_OMNIBOX_POPUP_VIEW_VIEWS_H_
