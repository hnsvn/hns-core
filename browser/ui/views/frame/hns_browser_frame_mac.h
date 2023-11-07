/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_MAC_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_MAC_H_

#include "chrome/browser/ui/views/frame/browser_frame_mac.h"

class Browser;

class HnsBrowserFrameMac : public BrowserFrameMac {
 public:
  HnsBrowserFrameMac(BrowserFrame* browser_frame, BrowserView* browser_view);
  ~HnsBrowserFrameMac() override;

  // BrowserFrameMac:
  void GetWindowFrameTitlebarHeight(bool* override_titlebar_height,
                                    float* titlebar_height) override;
  void ValidateUserInterfaceItem(
      int32_t command,
      remote_cocoa::mojom::ValidateUserInterfaceItemResult* result) override;

 private:
  raw_ptr<Browser> browser_;
  raw_ptr<BrowserView> browser_view_;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_MAC_H_
