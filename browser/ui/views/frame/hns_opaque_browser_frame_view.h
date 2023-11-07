/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_OPAQUE_BROWSER_FRAME_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_OPAQUE_BROWSER_FRAME_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"

class HnsWindowFrameGraphic;

class HnsOpaqueBrowserFrameView : public OpaqueBrowserFrameView {
 public:
  HnsOpaqueBrowserFrameView(BrowserFrame* frame,
                              BrowserView* browser_view,
                              OpaqueBrowserFrameViewLayout* layout);
  ~HnsOpaqueBrowserFrameView() override;

  HnsOpaqueBrowserFrameView(const HnsOpaqueBrowserFrameView&) = delete;
  HnsOpaqueBrowserFrameView& operator=(
      const HnsOpaqueBrowserFrameView&) = delete;

  // OpaqueBrowserFrameView overrides:
  void OnPaint(gfx::Canvas* canvas) override;
  int NonClientHitTest(const gfx::Point& point) override;
  void UpdateCaptionButtonPlaceholderContainerBackground() override;
  void PaintClientEdge(gfx::Canvas* canvas) const override;

 private:
  std::unique_ptr<HnsWindowFrameGraphic> frame_graphic_;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_OPAQUE_BROWSER_FRAME_VIEW_H_
