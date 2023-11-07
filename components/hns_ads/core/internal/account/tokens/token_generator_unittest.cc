/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/token_generator.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsTokenGeneratorTest, Generate) {
  // Arrange
  const TokenGenerator token_generator;

  // Act
  const std::vector<cbr::Token> tokens = token_generator.Generate(5);

  // Assert
  const size_t count = tokens.size();
  EXPECT_EQ(5U, count);
}

TEST(HnsAdsTokenGeneratorTest, GenerateZero) {
  // Arrange
  const TokenGenerator token_generator;

  // Act
  const std::vector<cbr::Token> tokens = token_generator.Generate(0);

  // Assert
  EXPECT_TRUE(tokens.empty());
}

}  // namespace hns_ads
