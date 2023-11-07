/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/frame/hns_browser_root_view.h"

#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"

HnsBrowserRootView::HnsBrowserRootView(BrowserView* browser_view,
                                           views::Widget* widget)
    : BrowserRootView(browser_view, widget),
      browser_(browser_view->browser()) {}

HnsBrowserRootView::~HnsBrowserRootView() = default;

bool HnsBrowserRootView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return BrowserRootView::OnMouseWheel(event);

    // Bypass BrowserRootView::OnMouseWheel() to avoid tab cycling feature.
#if BUILDFLAG(IS_LINUX)
  if (!base::FeatureList::IsEnabled(
          tabs::features::kHnsChangeActiveTabOnScrollEvent)) {
    return RootView::OnMouseWheel(event);
  }
#endif

  // As vertical tabs are always in a scroll view, we should prefer scrolling
  // to tab cycling.
  if (tabs::utils::ShouldShowVerticalTabs(browser_)) {
    return RootView::OnMouseWheel(event);
  }

  return BrowserRootView::OnMouseWheel(event);
}
