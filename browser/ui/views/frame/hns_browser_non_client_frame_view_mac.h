/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_

#include <memory>

#include "chrome/browser/ui/views/frame/browser_non_client_frame_view_mac.h"

class HnsWindowFrameGraphic;

class HnsBrowserNonClientFrameViewMac : public BrowserNonClientFrameViewMac {
 public:
  HnsBrowserNonClientFrameViewMac(BrowserFrame* frame,
                                    BrowserView* browser_view);
  ~HnsBrowserNonClientFrameViewMac() override;

  HnsBrowserNonClientFrameViewMac(
      const HnsBrowserNonClientFrameViewMac&) = delete;
  HnsBrowserNonClientFrameViewMac& operator=(
      const HnsBrowserNonClientFrameViewMac&) = delete;
  gfx::Size GetMinimumSize() const override;

 private:
  bool ShouldShowWindowTitleForVerticalTabs() const;
  void UpdateWindowTitleVisibility();
  void UpdateWindowTitleAndControls();

  // BrowserNonClientFrameViewMac overrides:
  void OnPaint(gfx::Canvas* canvas) override;
  int GetTopInset(bool restored) const override;
  int NonClientHitTest(const gfx::Point& point) override;

  std::unique_ptr<HnsWindowFrameGraphic> frame_graphic_;

  BooleanPrefMember show_vertical_tabs_;
  BooleanPrefMember show_title_bar_on_vertical_tabs_;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
