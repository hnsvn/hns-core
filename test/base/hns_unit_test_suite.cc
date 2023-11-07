/* Copyright (c) 2018 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/test/base/hns_unit_test_suite.h"

#include "base/logging.h"
#include "hns/common/resource_bundle_helper.h"
#include "hns/components/constants/hns_paths.h"
#include "build/build_config.h"
#include "chrome/install_static/product_install_details.h"
#include "chrome/test/base/chrome_unit_test_suite.h"

HnsUnitTestSuite::HnsUnitTestSuite(int argc, char** argv)
    : ChromeUnitTestSuite(argc, argv) {}

void HnsUnitTestSuite::Initialize() {
#if BUILDFLAG(IS_WIN) && defined(OFFICIAL_BUILD)
  // When ChromeExtensionsBrowserClient is initialized, it needs
  install_static::InitializeProductDetailsForPrimaryModule();
#endif
  ChromeUnitTestSuite::Initialize();

  hns::InitializeResourceBundle();

  hns::RegisterPathProvider();
}
