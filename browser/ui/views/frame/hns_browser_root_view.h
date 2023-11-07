/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_ROOT_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_ROOT_VIEW_H_

#include "chrome/browser/ui/views/frame/browser_root_view.h"

class Browser;

class HnsBrowserRootView : public BrowserRootView {
 public:
  HnsBrowserRootView(BrowserView* browser_view, views::Widget* widget);
  ~HnsBrowserRootView() override;

  // BrowserRootView:
  bool OnMouseWheel(const ui::MouseWheelEvent& event) override;

 private:
  raw_ptr<Browser> browser_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_ROOT_VIEW_H_
