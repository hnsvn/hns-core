/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_VIEW_WIN_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_VIEW_WIN_H_

#include <memory>

#include "chrome/browser/ui/views/frame/browser_frame_view_win.h"
#include "components/prefs/pref_member.h"

class HnsWindowFrameGraphic;

class HnsBrowserFrameViewWin : public BrowserFrameViewWin {
 public:
  HnsBrowserFrameViewWin(BrowserFrame* frame, BrowserView* browser_view);
  ~HnsBrowserFrameViewWin() override;

  HnsBrowserFrameViewWin(const HnsBrowserFrameViewWin&) = delete;
  HnsBrowserFrameViewWin& operator=(const HnsBrowserFrameViewWin&) = delete;

 private:
  void OnVerticalTabsPrefsChanged();

  // HnsBrowserFrameViewWin overrides:
  void OnPaint(gfx::Canvas* canvas) override;
  int GetTopInset(bool restored) const override;
  int NonClientHitTest(const gfx::Point& point) override;

  std::unique_ptr<HnsWindowFrameGraphic> frame_graphic_;

  BooleanPrefMember using_vertical_tabs_;
  BooleanPrefMember showing_window_title_for_vertical_tabs_;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_VIEW_WIN_H_
