/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_BROWSER_BROWSER_MANAGER_OBSERVER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_BROWSER_BROWSER_MANAGER_OBSERVER_H_

#include "base/observer_list_types.h"

namespace hns_ads {

class BrowserManagerObserver : public base::CheckedObserver {
 public:
  // Invoked when the browser becomes active.
  virtual void OnBrowserDidBecomeActive() {}

  // Invoked when the browser becomes inactive.
  virtual void OnBrowserDidResignActive() {}

  // Invoked when the browser enters the foreground.
  virtual void OnBrowserDidEnterForeground() {}

  // Invoked when the browser enters the background.
  virtual void OnBrowserDidEnterBackground() {}
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_BROWSER_BROWSER_MANAGER_OBSERVER_H_
