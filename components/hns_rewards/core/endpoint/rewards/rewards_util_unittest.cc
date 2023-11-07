/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/rewards/rewards_util.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=RewardsUtilTest.*

namespace hns_rewards::internal {
namespace endpoint {
namespace rewards {

class RewardsUtilTest : public testing::Test {};

TEST(RewardsUtilTest, GetServerUrlDevelopment) {
  _environment = mojom::Environment::DEVELOPMENT;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://rewards-dev.hns.software/test");
}

TEST(RewardsUtilTest, GetServerUrlStaging) {
  _environment = mojom::Environment::STAGING;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://rewards-stg.hnssoftware.com/test");
}

TEST(RewardsUtilTest, GetServerUrlProduction) {
  _environment = mojom::Environment::PRODUCTION;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://rewards.hns.com/test");
}

}  // namespace rewards
}  // namespace endpoint
}  // namespace hns_rewards::internal
