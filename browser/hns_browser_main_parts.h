/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_BROWSER_MAIN_PARTS_H_
#define HNS_BROWSER_HNS_BROWSER_MAIN_PARTS_H_

#include "chrome/browser/chrome_browser_main.h"

class HnsBrowserMainParts : public ChromeBrowserMainParts {
 public:
  using ChromeBrowserMainParts::ChromeBrowserMainParts;

  HnsBrowserMainParts(const HnsBrowserMainParts&) = delete;
  HnsBrowserMainParts& operator=(const HnsBrowserMainParts&) = delete;
  ~HnsBrowserMainParts() override = default;

  void PreBrowserStart() override;
  void PostBrowserStart() override;
  void PreShutdown() override;
  void PreProfileInit() override;
  void PostProfileInit(Profile* profile, bool is_initial_profile) override;

 private:
  friend class ChromeBrowserMainExtraPartsTor;
};

#endif  // HNS_BROWSER_HNS_BROWSER_MAIN_PARTS_H_
