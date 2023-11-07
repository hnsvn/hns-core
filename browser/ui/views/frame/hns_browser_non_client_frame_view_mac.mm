/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>

#include "hns/browser/ui/views/frame/hns_browser_non_client_frame_view_mac.h"

#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/frame/hns_non_client_hit_test_helper.h"
#include "hns/browser/ui/views/frame/hns_window_frame_graphic.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/view_ids.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/base/hit_test.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/scoped_canvas.h"

HnsBrowserNonClientFrameViewMac::HnsBrowserNonClientFrameViewMac(
    BrowserFrame* frame, BrowserView* browser_view)
    : BrowserNonClientFrameViewMac(frame, browser_view) {
  auto* browser = browser_view->browser();
  frame_graphic_ =
      std::make_unique<HnsWindowFrameGraphic>(browser->profile());

  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return;

  if (tabs::utils::SupportsVerticalTabs(browser)) {
    auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
    show_vertical_tabs_.Init(
        hns_tabs::kVerticalTabsEnabled, prefs,
        base::BindRepeating(
            &HnsBrowserNonClientFrameViewMac::UpdateWindowTitleAndControls,
            base::Unretained(this)));
    show_title_bar_on_vertical_tabs_.Init(
        hns_tabs::kVerticalTabsShowTitleOnWindow, prefs,
        base::BindRepeating(
            &HnsBrowserNonClientFrameViewMac::UpdateWindowTitleVisibility,
            base::Unretained(this)));
  }
}

HnsBrowserNonClientFrameViewMac::~HnsBrowserNonClientFrameViewMac() = default;

void HnsBrowserNonClientFrameViewMac::OnPaint(gfx::Canvas* canvas) {
  BrowserNonClientFrameViewMac::OnPaint(canvas);

  // Don't draw frame graphic over border outline.
  gfx::ScopedCanvas scoped_canvas(canvas);
  gfx::Rect bounds_to_frame_graphic(bounds());
  if (!IsFrameCondensed()) {
    // Native frame has 1px border outline.
    constexpr int kFrameBorderOutlineThickness = 1;
    bounds_to_frame_graphic.Inset(gfx::Insets::VH(
        kFrameBorderOutlineThickness, kFrameBorderOutlineThickness));
    canvas->ClipRect(bounds_to_frame_graphic);
  }
  frame_graphic_->Paint(canvas, bounds_to_frame_graphic);
}

int HnsBrowserNonClientFrameViewMac::GetTopInset(bool restored) const {
  if (ShouldShowWindowTitleForVerticalTabs()) {
    // Set minimum top inset to show caption buttons on frame.
    return 30;
  }

  return BrowserNonClientFrameViewMac::GetTopInset(restored);
}

bool HnsBrowserNonClientFrameViewMac::ShouldShowWindowTitleForVerticalTabs()
    const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return false;

  return tabs::utils::ShouldShowWindowTitleForVerticalTabs(
      browser_view()->browser());
}

void HnsBrowserNonClientFrameViewMac::UpdateWindowTitleVisibility() {
  DCHECK(base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
      << "This method should be called only when the flag is on.";

  if (!browser_view()->browser()->is_type_normal())
    return;

  frame()->SetWindowTitleVisibility(ShouldShowWindowTitleForVerticalTabs());
}

int HnsBrowserNonClientFrameViewMac::NonClientHitTest(
    const gfx::Point& point) {
  if (auto res = hns::NonClientHitTest(browser_view(), point);
      res != HTNOWHERE) {
    return res;
  }

  return BrowserNonClientFrameViewMac::NonClientHitTest(point);
}

void HnsBrowserNonClientFrameViewMac::UpdateWindowTitleAndControls() {
  UpdateWindowTitleVisibility();

  // In case title visibility wasn't changed and only vertical tab strip enabled
  // state changed, we should reset controls positions manually.
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&views::Widget::ResetWindowControlsPosition,
                                frame()->GetWeakPtr()));
}

gfx::Size HnsBrowserNonClientFrameViewMac::GetMinimumSize() const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return BrowserNonClientFrameViewMac::GetMinimumSize();

  if (tabs::utils::ShouldShowVerticalTabs(browser_view()->browser())) {
    // In order to ignore tab strip height, skip BrowserNonClientFrameViewMac's
    // implementation.
    auto size = frame()->client_view()->GetMinimumSize();
    size.SetToMax(gfx::Size(0, (size.width() * 3) / 4));
    // Note that we can't set empty bounds on Mac.
    size.SetToMax({1, 1});
    return size;
  }

  return BrowserNonClientFrameViewMac::GetMinimumSize();
}
