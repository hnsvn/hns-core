/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_util.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

TEST(HnsAdsTokenUtilTest, TokensToRawTokens) {
  // Arrange
  const std::vector<Token> tokens = GetTokensForTesting();

  // Act
  const std::vector<challenge_bypass_ristretto::Token> raw_tokens =
      ToRawTokens(tokens);

  // Assert
  std::vector<challenge_bypass_ristretto::Token> expected_raw_tokens;
  expected_raw_tokens.reserve(tokens.size());
  for (const auto& token : tokens) {
    expected_raw_tokens.push_back(token.get());
  }

  EXPECT_EQ(expected_raw_tokens, raw_tokens);
}

TEST(HnsAdsTokenUtilTest, EmptyTokensToRawTokens) {
  // Arrange
  const std::vector<Token> tokens;

  // Act
  const std::vector<challenge_bypass_ristretto::Token> raw_tokens =
      ToRawTokens(tokens);

  // Assert
  EXPECT_TRUE(raw_tokens.empty());
}

}  // namespace hns_ads::cbr
