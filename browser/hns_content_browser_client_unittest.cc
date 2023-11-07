/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_content_browser_client.h"

#include "base/memory/raw_ptr.h"
#include "base/values.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"
#include "testing/gtest/include/gtest/gtest.h"

#if BUILDFLAG(ENABLE_EXTENSIONS) || BUILDFLAG(IS_ANDROID)
#include "extensions/common/constants.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/browser/extension_registry.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_builder.h"
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED) && BUILDFLAG(ENABLE_EXTENSIONS)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "components/prefs/pref_service.h"
#include "content/public/common/content_client.h"

namespace extensions {

class HnsWalleBrowserClientUnitTest
    : public ChromeRenderViewHostTestHarness {
 public:
  HnsWalleBrowserClientUnitTest() = default;
  HnsWalleBrowserClientUnitTest(const HnsWalleBrowserClientUnitTest&) =
      delete;
  HnsWalleBrowserClientUnitTest& operator=(
      const HnsWalleBrowserClientUnitTest&) = delete;

  void SetUp() override {
    ChromeRenderViewHostTestHarness::SetUp();
    original_client_ = content::SetBrowserClientForTesting(&client_);
  }

  void TearDown() override {
    content::SetBrowserClientForTesting(original_client_);
    ChromeRenderViewHostTestHarness::TearDown();
  }

  void AddExtension() {
    extension_ = ExtensionBuilder()
                     .SetManifest(base::Value::Dict()
                                      .Set("name", "ext")
                                      .Set("version", "0.1")
                                      .Set("manifest_version", 2))
                     .SetID(ethereum_remote_client_extension_id)
                     .Build();
    ASSERT_TRUE(extension_);
    ExtensionRegistry::Get(browser_context())->AddReady(extension_.get());
  }

 private:
  scoped_refptr<const Extension> extension_;
  content::ContentBrowserClient client_;
  raw_ptr<content::ContentBrowserClient> original_client_ = nullptr;
};

TEST_F(HnsWalleBrowserClientUnitTest,
    DoesNotResolveEthereumRemoteClientIfNotInstalled) {
  GURL url("chrome://wallet/");
  ASSERT_FALSE(HnsContentBrowserClient::HandleURLOverrideRewrite(
        &url, browser_context()));
}

TEST_F(HnsWalleBrowserClientUnitTest,
    ResolvesEthereumRemoteClientIfInstalled) {
  AddExtension();
  profile()->GetPrefs()->SetInteger(
      kDefaultEthereumWallet,
      static_cast<int>(hns_wallet::mojom::DefaultWallet::CryptoWallets));
  GURL url("chrome://wallet/");
  ASSERT_TRUE(HnsContentBrowserClient::HandleURLOverrideRewrite(
        &url, browser_context()));
  ASSERT_STREQ(url.spec().c_str(), ethereum_remote_client_base_url);
}

}  // namespace extensions
#endif

using HnsContentBrowserClientTest = testing::Test;

TEST_F(HnsContentBrowserClientTest, ResolvesSync) {
  GURL url("chrome://sync/");
  ASSERT_TRUE(
    HnsContentBrowserClient::HandleURLOverrideRewrite(&url, nullptr));
  ASSERT_STREQ(url.spec().c_str(), "chrome://settings/hnsSync");

  GURL url2("chrome://sync/");
  ASSERT_TRUE(
    HnsContentBrowserClient::HandleURLOverrideRewrite(&url2, nullptr));
}

TEST_F(HnsContentBrowserClientTest, ResolvesWelcomePage) {
  GURL url("chrome://welcome/");
  ASSERT_TRUE(
      HnsContentBrowserClient::HandleURLOverrideRewrite(&url, nullptr));
}
