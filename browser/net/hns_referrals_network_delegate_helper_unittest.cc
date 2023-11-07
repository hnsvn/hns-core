/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_referrals_network_delegate_helper.h"

#include <memory>
#include <string>
#include <tuple>

#include "base/json/json_reader.h"
#include "hns/browser/net/url_context.h"
#include "hns/components/constants/network_constants.h"
#include "net/base/net_errors.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"
#include "url/url_constants.h"

using hns::ResponseCallback;

TEST(HnsReferralsNetworkDelegateHelperTest, ReplaceHeadersForMatchingDomain) {
  const std::array<std::tuple<GURL, std::string>, 1> test_cases = {
      std::make_tuple<>(GURL("http://grammarly.com"), "grammarly"),
  };

  for (const auto& c : test_cases) {
    net::HttpRequestHeaders headers;
    auto request_info =
        std::make_shared<hns::HnsRequestInfo>(std::get<0>(c));

    int rc = hns::OnBeforeStartTransaction_ReferralsWork(
        &headers, hns::ResponseCallback(), request_info);

    std::string partner_header;
    headers.GetHeader("X-Hns-Partner", &partner_header);
    EXPECT_EQ(partner_header, std::get<1>(c));
    EXPECT_EQ(rc, net::OK);
  }
}

TEST(HnsReferralsNetworkDelegateHelperTest,
     NoReplaceHeadersForNonMatchingDomain) {
  const std::array<GURL, 2> test_cases = {
      GURL("https://api-sandbox.uphold.com"),
      GURL("https://www.google.com"),
  };

  for (const auto& c : test_cases) {
    net::HttpRequestHeaders headers;

    auto request_info = std::make_shared<hns::HnsRequestInfo>(c);
    int rc = hns::OnBeforeStartTransaction_ReferralsWork(
        &headers, hns::ResponseCallback(), request_info);

    EXPECT_FALSE(headers.HasHeader("X-Hns-Partner"));
    EXPECT_EQ(rc, net::OK);
  }
}
