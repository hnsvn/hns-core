/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_browser_main_loop.h"

#include "content/public/browser/browser_main_parts.h"

namespace hns {

void HnsBrowserMainLoop::PreShutdown() {
  parts()->PreShutdown();
  content::BrowserMainLoop::PreShutdown();
}

}  // namespace hns
