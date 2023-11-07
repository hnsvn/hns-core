/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_

#define LayoutWindowControlsOverlay               \
  LayoutWindowControlsOverlay_Unused() {}         \
  friend class HnsBrowserNonClientFrameViewMac; \
  void LayoutWindowControlsOverlay

#include "src/chrome/browser/ui/views/frame/browser_non_client_frame_view_mac.h"  // IWYU pragma: export

#undef LayoutWindowControlsOverlay

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
