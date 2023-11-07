/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_APP_HNS_MAIN_DELEGATE_H_
#define HNS_APP_HNS_MAIN_DELEGATE_H_

#include "build/build_config.h"
#include "chrome/app/chrome_main_delegate.h"

// Chrome implementation of ContentMainDelegate.
class HnsMainDelegate : public ChromeMainDelegate {
 public:
  HnsMainDelegate(const HnsMainDelegate&) = delete;
  HnsMainDelegate& operator=(const HnsMainDelegate&) = delete;
  HnsMainDelegate();

  // |exe_entry_point_ticks| is the time at which the main function of the
  // executable was entered, or null if not available.
  explicit HnsMainDelegate(base::TimeTicks exe_entry_point_ticks);
  ~HnsMainDelegate() override;

 protected:
  // content::ContentMainDelegate implementation:
  content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;
  content::ContentUtilityClient* CreateContentUtilityClient() override;
  void PreSandboxStartup() override;
  absl::optional<int> PostEarlyInitialization(
      ChromeMainDelegate::InvokedIn invoked_in) override;
};

#endif  // HNS_APP_HNS_MAIN_DELEGATE_H_
