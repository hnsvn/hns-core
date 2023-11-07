/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/private_cdn/private_cdn_util.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=PrivateCDNTest.*

namespace hns_rewards::internal {
namespace endpoint {
namespace private_cdn {

class PrivateCDNTest : public testing::Test {};

TEST(PrivateCDNTest, GetServerUrlDevelopment) {
  _environment = mojom::Environment::DEVELOPMENT;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://pcdn.hns.software/test");
}

TEST(PrivateCDNTest, GetServerUrlStaging) {
  _environment = mojom::Environment::STAGING;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://pcdn.hnssoftware.com/test");
}

TEST(PrivateCDNTest, GetServerUrlProduction) {
  _environment = mojom::Environment::PRODUCTION;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://pcdn.hns.com/test");
}

}  // namespace private_cdn
}  // namespace endpoint
}  // namespace hns_rewards::internal
