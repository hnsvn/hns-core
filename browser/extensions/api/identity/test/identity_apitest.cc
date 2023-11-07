/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "hns/browser/extensions/api/identity/hns_web_auth_flow.h"
#include "hns/components/constants/hns_paths.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "content/public/test/browser_test.h"
#include "extensions/browser/background_script_executor.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {
constexpr char kIdentityTestExtensionId[] = "igbmfgdcighdkjdgcnoaboocnjopojdh";

class IdentityExtensionApiTest : public ExtensionApiTest {
 public:
  IdentityExtensionApiTest() = default;
  void SetUp() override {
    hns::RegisterPathProvider();
    base::PathService::Get(hns::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }

  void TearDown() override { ExtensionApiTest::TearDown(); }

  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(IdentityExtensionApiTest, FetchingTokenInteractiveMode) {
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("getAuthToken"));
  ASSERT_TRUE(extension);
  HnsWebAuthFlow::SetTokenForTesting("test_token");

  ASSERT_TRUE(extensions::BackgroundScriptExecutor::ExecuteScriptAsync(
      browser()->profile(), kIdentityTestExtensionId, R"(
        chrome.identity.getAuthToken({ interactive: true }, function(token) {
          if (chrome.runtime.lastError) {
            chrome.test.fail();
            return;
          }
          if (token === "test_token") {
            chrome.test.succeed();
          } else {
            chrome.test.fail();
          }
        });
      )",
      browsertest_util::ScriptUserActivation::kDontActivate));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(IdentityExtensionApiTest, FetchingTokenSilentMode) {
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("getAuthToken"));
  ASSERT_TRUE(extension);
  HnsWebAuthFlow::SetTokenForTesting("test_token");

  ASSERT_TRUE(extensions::BackgroundScriptExecutor::ExecuteScriptAsync(
      browser()->profile(), kIdentityTestExtensionId, R"(
        chrome.identity.getAuthToken({ interactive: false }, function(token) {
          if (chrome.runtime.lastError) {
            chrome.test.fail();
            return;
          }
          if (token === "test_token") {
            chrome.test.succeed();
          } else {
            chrome.test.fail();
          }
        });
      )",
      browsertest_util::ScriptUserActivation::kDontActivate));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

}  // namespace
}  // namespace extensions
