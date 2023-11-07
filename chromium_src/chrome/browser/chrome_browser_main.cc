/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/chrome_browser_main.h"
#include "hns/browser/hns_browser_process_impl.h"

#define BrowserProcessImpl HnsBrowserProcessImpl
#include "src/chrome/browser/chrome_browser_main.cc"
#undef BrowserProcessImpl
