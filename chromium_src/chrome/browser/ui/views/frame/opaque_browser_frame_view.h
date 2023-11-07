/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_

#define WebAppOpaqueBrowserFrameViewWindowControlsOverlayTest \
  WebAppOpaqueBrowserFrameViewWindowControlsOverlayTest;      \
  friend class HnsOpaqueBrowserFrameView
#define UpdateCaptionButtonPlaceholderContainerBackground \
  virtual UpdateCaptionButtonPlaceholderContainerBackground
#define PaintClientEdge virtual PaintClientEdge

#include "src/chrome/browser/ui/views/frame/opaque_browser_frame_view.h"  // IWYU pragma: export

#undef PaintClientEdge
#undef UpdateCaptionButtonPlaceholderContainerBackground
#undef WebAppOpaqueBrowserFrameViewWindowControlsOverlayTest

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_
