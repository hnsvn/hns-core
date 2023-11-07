/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_TEST_BASE_HNS_TEST_LAUNCHER_DELEGATE_H_
#define HNS_TEST_BASE_HNS_TEST_LAUNCHER_DELEGATE_H_

#include "chrome/test/base/chrome_test_launcher.h"

class HnsTestLauncherDelegate : public ChromeTestLauncherDelegate {
 public:
  // Does not take ownership of ChromeTestSuiteRunner.
  explicit HnsTestLauncherDelegate(ChromeTestSuiteRunner* runner);
  HnsTestLauncherDelegate(const HnsTestLauncherDelegate&) = delete;
  HnsTestLauncherDelegate& operator=(const HnsTestLauncherDelegate&) =
      delete;
  ~HnsTestLauncherDelegate() override;

 private:
  // ChromeLauncherDelegate:
  content::ContentMainDelegate* CreateContentMainDelegate() override;
};

#endif  // HNS_TEST_BASE_HNS_TEST_LAUNCHER_DELEGATE_H_
