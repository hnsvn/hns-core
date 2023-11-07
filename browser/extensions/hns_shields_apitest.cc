/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "hns/components/constants/hns_paths.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "content/public/test/browser_test.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {

class HnsShieldsExtensionApiTest : public ExtensionApiTest {
 public:
  void SetUp() override {
    hns::RegisterPathProvider();
    base::PathService::Get(hns::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }
  void TearDown() override {
    ExtensionApiTest::TearDown();
  }
  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(HnsShieldsExtensionApiTest, HnsExtensionHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("hnsShields"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsShieldsExtensionApiTest,
    NotHnsExtensionHasNoAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("notHnsShields"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

}  // namespace
}  // namespace extensions
