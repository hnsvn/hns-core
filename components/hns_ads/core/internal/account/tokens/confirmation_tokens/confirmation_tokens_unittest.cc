/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens.h"

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConfirmationTokensTest : public UnitTestBase {};

TEST_F(HnsAdsConfirmationTokensTest, GetToken) {
  // Arrange
  const ConfirmationTokenList tokens =
      BuildConfirmationTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens(tokens);

  // Act

  // Assert
  EXPECT_EQ(tokens.at(0), confirmation_tokens.GetToken());
}

TEST_F(HnsAdsConfirmationTokensTest, GetAllTokens) {
  // Arrange
  const ConfirmationTokenList tokens =
      BuildConfirmationTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens(tokens);

  // Act

  // Assert
  EXPECT_EQ(tokens, confirmation_tokens.GetAllTokens());
}

TEST_F(HnsAdsConfirmationTokensTest, SetTokens) {
  // Arrange
  const ConfirmationTokenList tokens =
      BuildConfirmationTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  ConfirmationTokens confirmation_tokens;

  // Act
  confirmation_tokens.SetTokens(tokens);

  // Assert
  EXPECT_EQ(tokens, confirmation_tokens.GetAllTokens());
}

TEST_F(HnsAdsConfirmationTokensTest, SetEmptyTokens) {
  // Arrange
  ConfirmationTokens confirmation_tokens;

  // Act
  confirmation_tokens.SetTokens({});

  // Assert
  EXPECT_TRUE(confirmation_tokens.IsEmpty());
}

TEST_F(HnsAdsConfirmationTokensTest, AddTokens) {
  // Arrange
  const ConfirmationTokenList tokens =
      BuildConfirmationTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens({tokens.at(0)});

  // Act
  confirmation_tokens.AddTokens({tokens.at(1)});

  // Assert
  EXPECT_EQ(2, confirmation_tokens.Count());
}

TEST_F(HnsAdsConfirmationTokensTest, AddEmptyTokens) {
  // Arrange
  ConfirmationTokens confirmation_tokens;

  // Act
  confirmation_tokens.AddTokens({});

  // Assert
  EXPECT_TRUE(confirmation_tokens.IsEmpty());
}

TEST_F(HnsAdsConfirmationTokensTest, DoNotAddDuplicateTokens) {
  // Arrange
  const ConfirmationTokenInfo confirmation_token =
      BuildConfirmationTokenForTesting();

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.AddTokens({confirmation_token});

  // Act
  confirmation_tokens.AddTokens({confirmation_token});

  // Assert
  EXPECT_EQ(1, confirmation_tokens.Count());
}

TEST_F(HnsAdsConfirmationTokensTest, RemoveToken) {
  // Arrange
  const ConfirmationTokenList tokens =
      BuildConfirmationTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens(tokens);

  const ConfirmationTokenInfo& token_1 = tokens.at(0);
  const ConfirmationTokenInfo& token_2 = tokens.at(1);

  // Act
  confirmation_tokens.RemoveToken(token_2);

  // Assert
  const ConfirmationTokenList expected_tokens = {token_1};
  EXPECT_EQ(expected_tokens, confirmation_tokens.GetAllTokens());
}

TEST_F(HnsAdsConfirmationTokensTest, RemoveTokens) {
  // Arrange
  const ConfirmationTokenList tokens =
      BuildConfirmationTokensForTesting(/*count*/ 3);
  ASSERT_EQ(3U, tokens.size());

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens(tokens);

  const ConfirmationTokenInfo& token_1 = tokens.at(0);
  const ConfirmationTokenInfo& token_2 = tokens.at(1);
  const ConfirmationTokenInfo& token_3 = tokens.at(2);

  // Act
  confirmation_tokens.RemoveTokens({token_1, token_3});

  // Assert
  const ConfirmationTokenList expected_tokens = {token_2};
  EXPECT_EQ(expected_tokens, confirmation_tokens.GetAllTokens());
}

TEST_F(HnsAdsConfirmationTokensTest, RemoveAllTokens) {
  // Arrange
  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens(BuildConfirmationTokensForTesting(/*count*/ 2));

  // Act
  confirmation_tokens.RemoveAllTokens();

  // Assert
  EXPECT_TRUE(confirmation_tokens.IsEmpty());
}

TEST_F(HnsAdsConfirmationTokensTest, TokenDoesExist) {
  // Arrange
  const ConfirmationTokenInfo confirmation_token =
      BuildConfirmationTokenForTesting();

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens({confirmation_token});

  // Act

  // Assert
  EXPECT_TRUE(confirmation_tokens.TokenExists(confirmation_token));
}

TEST_F(HnsAdsConfirmationTokensTest, TokenDoesNotExist) {
  // Arrange
  ConfirmationTokens confirmation_tokens;

  // Act

  // Assert
  EXPECT_FALSE(
      confirmation_tokens.TokenExists(BuildConfirmationTokenForTesting()));
}

TEST_F(HnsAdsConfirmationTokensTest, Count) {
  // Arrange
  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens(BuildConfirmationTokensForTesting(/*count*/ 3));

  // Act

  // Assert
  EXPECT_EQ(3, confirmation_tokens.Count());
}

TEST_F(HnsAdsConfirmationTokensTest, IsEmpty) {
  // Arrange
  const ConfirmationTokens confirmation_tokens;

  // Act

  // Assert
  EXPECT_TRUE(confirmation_tokens.IsEmpty());
}

TEST_F(HnsAdsConfirmationTokensTest, IsNotEmpty) {
  // Arrange
  const ConfirmationTokenInfo confirmation_token =
      BuildConfirmationTokenForTesting();

  ConfirmationTokens confirmation_tokens;
  confirmation_tokens.SetTokens({confirmation_token});

  // Act

  // Assert
  EXPECT_FALSE(confirmation_tokens.IsEmpty());
}

}  // namespace hns_ads
