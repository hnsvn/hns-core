/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_TEST_BASE_HNS_UNIT_TEST_SUITE_H_
#define HNS_TEST_BASE_HNS_UNIT_TEST_SUITE_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/test/test_discardable_memory_allocator.h"
#include "chrome/test/base/chrome_unit_test_suite.h"

class HnsUnitTestSuite : public ChromeUnitTestSuite {
 public:
  HnsUnitTestSuite(int argc, char** argv);
  HnsUnitTestSuite(const HnsUnitTestSuite&) = delete;
  HnsUnitTestSuite& operator=(const HnsUnitTestSuite&) = delete;

 protected:
  // base::TestSuite overrides:
  void Initialize() override;
};

#endif  // HNS_TEST_BASE_HNS_UNIT_TEST_SUITE_H_
