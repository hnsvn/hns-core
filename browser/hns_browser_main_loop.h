/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_BROWSER_MAIN_LOOP_H_
#define HNS_BROWSER_HNS_BROWSER_MAIN_LOOP_H_

#include "content/browser/browser_main_loop.h"

namespace hns {

class HnsBrowserMainLoop : public content::BrowserMainLoop {
 public:
  using BrowserMainLoop::BrowserMainLoop;

  HnsBrowserMainLoop(const HnsBrowserMainLoop&) = delete;
  HnsBrowserMainLoop& operator=(const HnsBrowserMainLoop&) = delete;
  ~HnsBrowserMainLoop() override = default;

  void PreShutdown() override;
};

}  // namespace hns

#endif  // HNS_BROWSER_HNS_BROWSER_MAIN_LOOP_H_
