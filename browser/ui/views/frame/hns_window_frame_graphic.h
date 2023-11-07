/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_WINDOW_FRAME_GRAPHIC_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_WINDOW_FRAME_GRAPHIC_H_

namespace gfx {
class Canvas;
class Rect;
}  // namespace gfx

namespace content {
class BrowserContext;
}  // namespace content

class HnsWindowFrameGraphic {
 public:
  explicit HnsWindowFrameGraphic(content::BrowserContext* context);
  virtual ~HnsWindowFrameGraphic();

  HnsWindowFrameGraphic(const HnsWindowFrameGraphic&) = delete;
  HnsWindowFrameGraphic& operator=(const HnsWindowFrameGraphic&) = delete;

  void Paint(gfx::Canvas* canvas, const gfx::Rect& frame_bounds);

 private:
  const bool is_tor_window_;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_WINDOW_FRAME_GRAPHIC_H_
