/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_browser.h"
#include "content/public/test/browser_test_base.h"

#define SetUpCommandLine(COMMAND_LINE) \
  SetUpCommandLine(COMMAND_LINE);      \
  HnsBrowser::SuppressBrowserWindowClosingDialogForTesting(true)
#include "src/chrome/test/base/in_process_browser_test.cc"
#undef SetUpCommandLine
