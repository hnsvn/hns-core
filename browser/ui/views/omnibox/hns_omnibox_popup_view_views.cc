/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/omnibox/hns_omnibox_popup_view_views.h"

#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/browser/ui/views/omnibox/rounded_omnibox_results_frame.h"
#include "ui/base/metadata/metadata_impl_macros.h"

HnsOmniboxPopupViewViews::~HnsOmniboxPopupViewViews() = default;

gfx::Rect HnsOmniboxPopupViewViews::GetTargetBounds() const {
  auto bounds = OmniboxPopupViewViews::GetTargetBounds();
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return bounds;
  }

  if (auto* browser = location_bar_view_->browser();
      tabs::utils::ShouldShowVerticalTabs(browser) &&
      !tabs::utils::ShouldShowWindowTitleForVerticalTabs(browser)) {
    // Remove top shadow inset so that omnibox popup stays inside browser
    // widget. Especially on Mac, Widgets can't be out of screen so we need to
    // adjust popup position.
    // https://github.com/hnsvn/hns-browser/issues/26573
    bounds.Inset(gfx::Insets().set_top(
        RoundedOmniboxResultsFrame::GetShadowInsets().top()));
  }

  return bounds;
}

BEGIN_METADATA(HnsOmniboxPopupViewViews, OmniboxPopupViewViews)
END_METADATA
