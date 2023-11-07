/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_LOOKALIKES_LOOKALIKE_URL_NAVIGATION_THROTTLE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_LOOKALIKES_LOOKALIKE_URL_NAVIGATION_THROTTLE_H_

#define MaybeCreateNavigationThrottle                                          \
  MaybeCreateNavigationThrottle(content::NavigationHandle* navigation_handle); \
  static std::unique_ptr<LookalikeUrlNavigationThrottle>                       \
      MaybeCreateNavigationThrottle_ChromiumImpl

#include "src/chrome/browser/lookalikes/lookalike_url_navigation_throttle.h"  // IWYU pragma: export
#undef MaybeCreateNavigationThrottle

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_LOOKALIKES_LOOKALIKE_URL_NAVIGATION_THROTTLE_H_
