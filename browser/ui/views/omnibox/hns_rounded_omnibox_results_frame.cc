/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/omnibox/hns_rounded_omnibox_results_frame.h"

#include <memory>
#include <utility>

#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "ui/base/metadata/metadata_impl_macros.h"

HnsRoundedOmniboxResultsFrame::HnsRoundedOmniboxResultsFrame(
    views::View* contents,
    LocationBarView* location_bar)
    : RoundedOmniboxResultsFrame(contents, location_bar),
      browser_(location_bar->browser()) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return;

  UpdateShadowBorder();

  show_vertical_tabs_.Init(
      hns_tabs::kVerticalTabsEnabled,
      browser_->profile()->GetOriginalProfile()->GetPrefs(),
      base::BindRepeating(&HnsRoundedOmniboxResultsFrame::UpdateShadowBorder,
                          base::Unretained(this)));
  show_window_title_for_vertical_tabs_.Init(
      hns_tabs::kVerticalTabsShowTitleOnWindow,
      browser_->profile()->GetOriginalProfile()->GetPrefs(),
      base::BindRepeating(&HnsRoundedOmniboxResultsFrame::UpdateShadowBorder,
                          base::Unretained(this)));
}

HnsRoundedOmniboxResultsFrame::~HnsRoundedOmniboxResultsFrame() = default;

void HnsRoundedOmniboxResultsFrame::UpdateShadowBorder() {
  DCHECK(base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs));

  int corner_radius = views::LayoutProvider::Get()->GetCornerRadiusMetric(
      views::Emphasis::kHigh);

  auto border = std::make_unique<views::BubbleBorder>(
      views::BubbleBorder::Arrow::NONE,
      views::BubbleBorder::Shadow::STANDARD_SHADOW);
  border->SetCornerRadius(corner_radius);
  border->set_md_shadow_elevation(GetShadowElevation());
  if (tabs::utils::ShouldShowVerticalTabs(browser_) &&
      !tabs::utils::ShouldShowWindowTitleForVerticalTabs(browser_)) {
    // Remove top shadow inset so that omnibox popup stays inside browser
    // widget. Especially on Mac, Widgets can't be out of screen so we need to
    // adjust popup position.
    // https://github.com/hnsvn/hns-browser/issues/26573
    border->set_insets(
        RoundedOmniboxResultsFrame::GetShadowInsets().set_top(0));
  }
  SetBorder(std::move(border));
}

BEGIN_METADATA(HnsRoundedOmniboxResultsFrame, RoundedOmniboxResultsFrame)
END_METADATA
