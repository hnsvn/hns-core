/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_BROWSER_MAIN_EXTRA_PARTS_H_
#define HNS_BROWSER_HNS_BROWSER_MAIN_EXTRA_PARTS_H_

#include "base/compiler_specific.h"
#include "chrome/browser/chrome_browser_main.h"
#include "chrome/browser/chrome_browser_main_extra_parts.h"

class HnsBrowserMainExtraParts : public ChromeBrowserMainExtraParts {
 public:
  HnsBrowserMainExtraParts();
  HnsBrowserMainExtraParts(const HnsBrowserMainExtraParts&) = delete;
  HnsBrowserMainExtraParts& operator=(const HnsBrowserMainExtraParts&) =
      delete;
  ~HnsBrowserMainExtraParts() override;

  // ChromeBrowserMainExtraParts overrides.
  void PostBrowserStart() override;
  void PreMainMessageLoopRun() override;
  void PreProfileInit() override;
};

#endif  // HNS_BROWSER_HNS_BROWSER_MAIN_EXTRA_PARTS_H_
