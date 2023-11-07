/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_IMPL_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_IMPL_H_

// Make HnsStartupBrowserCreatorImpl a friend class, to give it access to
// StartupBrowserCreatorImpl's private attributes.
#define browser_creator_ \
  browser_creator_;      \
  friend class HnsStartupBrowserCreatorImpl
#include "src/chrome/browser/ui/startup/startup_browser_creator_impl.h"  // IWYU pragma: export
#undef browser_creator_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_IMPL_H_
