/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "hns/browser/net/hns_httpse_network_delegate_helper.h"

#include "hns/browser/net/url_context.h"
#include "hns/components/constants/network_constants.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "content/public/test/browser_task_environment.h"
#include "net/cookies/site_for_cookies.h"
#include "net/traffic_annotation/network_traffic_annotation_test_helper.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_builder.h"
#include "net/url_request/url_request_test_util.h"

namespace {

class HnsHTTPSENetworkDelegateHelperTest: public testing::Test {
 public:
  HnsHTTPSENetworkDelegateHelperTest()
      : task_environment_(content::BrowserTaskEnvironment::IO_MAINLOOP) {}
  ~HnsHTTPSENetworkDelegateHelperTest() override = default;
  void SetUp() override {
    context_ = net::CreateTestURLRequestContextBuilder()->Build();
  }
  net::URLRequestContext* context() { return context_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<net::URLRequestContext> context_;
};


TEST_F(HnsHTTPSENetworkDelegateHelperTest, AlreadySetNewURLNoOp) {
  net::TestDelegate test_delegate;
  GURL url("http://bradhatesprimes.hns.com/composite_numbers_ftw");
  std::unique_ptr<net::URLRequest> request =
      context()->CreateRequest(url, net::IDLE, &test_delegate,
                               TRAFFIC_ANNOTATION_FOR_TESTS);
  std::shared_ptr<hns::HnsRequestInfo>
      hns_request_info(new hns::HnsRequestInfo());
  request->set_site_for_cookies(net::SiteForCookies::FromUrl(
      GURL("http://brad.hns.com/hide_all_primes_in_ui/composites_forever")));
  hns_request_info->new_url_spec = "data:image/png;base64,iVB";
  hns::ResponseCallback callback;
  int ret =
    OnBeforeURLRequest_HttpsePreFileWork(callback, hns_request_info);
  EXPECT_EQ(hns_request_info->new_url_spec, hns_request_info->new_url_spec);
  EXPECT_EQ(ret, net::OK);
}

}  // namespace
