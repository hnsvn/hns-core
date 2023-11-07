/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_WIN_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_WIN_H_

#define client_view_bounds_ \
  client_view_bounds_;      \
  friend class HnsBrowserFrameViewWin

#include "src/chrome/browser/ui/views/frame/browser_frame_view_win.h"  // IWYU pragma: export
#undef client_view_bounds_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_WIN_H_
