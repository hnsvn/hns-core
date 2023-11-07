/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/chromium_src/chrome/browser/chrome_browser_main_win.h"

#include "hns/browser/hns_browser_main_parts.h"
#include "chrome/browser/chrome_browser_main.h"

#define ChromeBrowserMainParts HnsBrowserMainParts
#include "src/chrome/browser/chrome_browser_main_win.cc"
#undef ChromeBrowserMainParts
