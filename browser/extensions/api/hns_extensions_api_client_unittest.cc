/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <string>

#include "hns/browser/extensions/api/hns_extensions_api_client.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "extensions/browser/api/web_request/web_request_info.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace extensions {

class HnsExtensionsAPIClientTests : public ChromeRenderViewHostTestHarness {
 public:
  HnsExtensionsAPIClientTests(const HnsExtensionsAPIClientTests&) = delete;
  HnsExtensionsAPIClientTests& operator=(
      const HnsExtensionsAPIClientTests&) = delete;

  bool ShouldHideBrowserNetworkRequest(const WebRequestInfo& request_info) {
    return client_.ShouldHideBrowserNetworkRequest(browser_context(),
                                                   request_info);
  }

 protected:
  HnsExtensionsAPIClientTests() = default;

 private:
  HnsExtensionsAPIClient client_;
};

TEST_F(HnsExtensionsAPIClientTests, IsHnsProtectedUrlUphold) {
  auto create_request_params = [](const std::string& url) {
    const int kRendererProcessId = 2;
    WebRequestInfoInitParams request;
    request.url = GURL(url);
    request.render_process_id = kRendererProcessId;
    return request;
  };

  WebRequestInfo blocked1(
      create_request_params("https://uphold.com/authorize/1234"));
  WebRequestInfo blocked2(create_request_params(
      "https://wallet-sandbox.uphold.com/authorize/1234?state=abcd"));
  WebRequestInfo blocked3(
      create_request_params("https://api.uphold.com/oauth2/token"));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked1));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked2));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked3));

  WebRequestInfo allowed1(create_request_params("https://uphold.com/"));
  WebRequestInfo allowed2(create_request_params("https://www.uphold.com/"));
  WebRequestInfo allowed3(create_request_params("https://hns.com/"));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed1));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed2));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed3));
}

}  // namespace extensions
