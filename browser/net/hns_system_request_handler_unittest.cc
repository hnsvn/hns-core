/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_system_request_handler.h"

#include <string>

#include "services/network/public/cpp/resource_request.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns {

TEST(HnsSystemRequestHandlerTest, AddHnsServiceKeyHeaderForHns) {
  GURL url("https://demo.hns.com");
  network::ResourceRequest request;

  request.url = url;
  hns::AddHnsServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kHnsServicesKeyHeader, &key));
  EXPECT_EQ(key, HnsServicesKeyForTesting());
}

TEST(HnsSystemRequestHandlerTest, AddHnsServiceKeyHeaderForHnsSoftware) {
  GURL url("https://demo.hnssoftware.com");
  network::ResourceRequest request;

  request.url = url;
  hns::AddHnsServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kHnsServicesKeyHeader, &key));
  EXPECT_EQ(key, HnsServicesKeyForTesting());
}

TEST(HnsSystemRequestHandlerTest, DontAddHnsServiceKeyHeader) {
  GURL url("https://demo.example.com");
  network::ResourceRequest request;

  request.url = url;
  hns::AddHnsServicesKeyHeader(&request);
  std::string key;
  EXPECT_FALSE(request.headers.GetHeader(kHnsServicesKeyHeader, &key));
}

}  // namespace hns
