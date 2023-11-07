/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/frame/hns_browser_frame.h"
#include "hns/browser/ui/views/frame/hns_browser_view.h"
#include "chrome/browser/ui/views/frame/browser_view.h"

#define BrowserFrame HnsBrowserFrame
#define BrowserView HnsBrowserView
#include "src/chrome/browser/ui/views/frame/browser_window_factory.cc"
#undef BrowserView
#undef BrowserFrame
