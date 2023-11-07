/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_BROWSER_MAIN_PARTS_MAC_H_
#define HNS_BROWSER_HNS_BROWSER_MAIN_PARTS_MAC_H_

#include "chrome/browser/chrome_browser_main_mac.h"

class HnsBrowserMainPartsMac : public ChromeBrowserMainPartsMac {
 public:
  using ChromeBrowserMainPartsMac::ChromeBrowserMainPartsMac;
  ~HnsBrowserMainPartsMac() override = default;

 private:
  // ChromeBrowserMainPartsMac overrides:
  void PreCreateMainMessageLoop() override;
};

#endif  // HNS_BROWSER_HNS_BROWSER_MAIN_PARTS_MAC_H_
