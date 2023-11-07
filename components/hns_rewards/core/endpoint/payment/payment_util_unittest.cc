/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/payment/payment_util.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=PaymentUtilTest.*

namespace hns_rewards::internal {
namespace endpoint {
namespace payment {

class PaymentUtilTest : public testing::Test {};

TEST(PaymentUtilTest, GetServerUrlDevelopment) {
  _environment = mojom::Environment::DEVELOPMENT;
  const std::string url = GetServerUrl("/test");
  const std::string expected_url = "";
  ASSERT_EQ(url, "https://payment.rewards.hns.software/test");
}

TEST(PaymentUtilTest, GetServerUrlStaging) {
  _environment = mojom::Environment::STAGING;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://payment.rewards.hnssoftware.com/test");
}

TEST(PaymentUtilTest, GetServerUrlProduction) {
  _environment = mojom::Environment::PRODUCTION;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://payment.rewards.hns.com/test");
}

}  // namespace payment
}  // namespace endpoint
}  // namespace hns_rewards::internal
