/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/logging/logging_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=LoggingUtilTest.*

namespace hns_rewards::internal {

class LoggingUtilTest : public testing::Test {};

TEST(LoggingUtilTest, ShouldLogHeader) {
  EXPECT_TRUE(ShouldLogHeader("Content-Type: application/json; charset=UTF-8"));

  EXPECT_TRUE(ShouldLogHeader("Content-type: application/json; charset=UTF-8"));

  EXPECT_TRUE(ShouldLogHeader("digest: a527380a32beee78b46a"));

  EXPECT_TRUE(ShouldLogHeader("Digest: a527380a32beee78b46a"));

  EXPECT_FALSE(ShouldLogHeader("Authorization: Bearer a527380a32beee78b46a"));

  EXPECT_FALSE(ShouldLogHeader("authorization: Bearer a527380a32beee78b46a"));

  EXPECT_FALSE(ShouldLogHeader("Cookie: yummy_cookie=choco;"));

  EXPECT_FALSE(ShouldLogHeader("cookie: yummy_cookie=choco;"));
}

}  // namespace hns_rewards::internal
