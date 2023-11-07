/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_browser_main_parts_mac.h"

#include "hns/browser/sparkle_buildflags.h"

#if BUILDFLAG(ENABLE_SPARKLE)
#import "hns/browser/mac/sparkle_glue.h"
#endif

void HnsBrowserMainPartsMac::PreCreateMainMessageLoop() {
  ChromeBrowserMainPartsMac::PreCreateMainMessageLoop();

#if BUILDFLAG(ENABLE_SPARKLE)
  // It would be no-op if udpate is disabled.
  [[SparkleGlue sharedSparkleGlue] registerWithSparkle];
#endif
}
