/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_TEST_BASE_TESTING_BROWSER_PROCESS_H_
#define HNS_CHROMIUM_SRC_CHROME_TEST_BASE_TESTING_BROWSER_PROCESS_H_

#define TestingBrowserProcess TestingBrowserProcess_ChromiumImpl
#include "src/chrome/test/base/testing_browser_process.h"  // IWYU pragma: export
#undef TestingBrowserProcess

#include "hns/test/base/testing_hns_browser_process.h"

class TestingBrowserProcess : public TestingBrowserProcess_ChromiumImpl {
 public:
  // Initializes |g_browser_process| with a new TestingBrowserProcess.
  static void CreateInstance();

  // Cleanly destroys |g_browser_process|, which has special deletion semantics.
  static void DeleteInstance();

  // Convenience method to both teardown and destroy the TestingBrowserProcess
  // instance
  static void TearDownAndDeleteInstance();

  // Convenience method to get g_browser_process as a TestingBrowserProcess*.
  static TestingBrowserProcess* GetGlobal();

 private:
  TestingBrowserProcess();
  ~TestingBrowserProcess() override;
};

#endif  // HNS_CHROMIUM_SRC_CHROME_TEST_BASE_TESTING_BROWSER_PROCESS_H_
