/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens.h"

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPaymentTokensTest : public UnitTestBase {};

TEST_F(HnsAdsPaymentTokensTest, GetToken) {
  // Arrange
  const PaymentTokenList tokens = BuildPaymentTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(tokens);

  // Act

  // Assert
  const PaymentTokenInfo& expected_token = tokens.at(0);
  EXPECT_EQ(expected_token, payment_tokens.GetToken());
}

TEST_F(HnsAdsPaymentTokensTest, GetAllTokens) {
  // Arrange
  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(BuildPaymentTokensForTesting(/*count*/ 2));

  // Act

  // Assert
  EXPECT_EQ(BuildPaymentTokensForTesting(/*count*/ 2),
            payment_tokens.GetAllTokens());
}

TEST_F(HnsAdsPaymentTokensTest, SetTokens) {
  // Arrange
  PaymentTokens payment_tokens;

  // Act
  payment_tokens.SetTokens(BuildPaymentTokensForTesting(/*count*/ 2));

  // Assert
  EXPECT_EQ(BuildPaymentTokensForTesting(/*count*/ 2),
            payment_tokens.GetAllTokens());
}

TEST_F(HnsAdsPaymentTokensTest, SetEmptyTokens) {
  // Arrange
  PaymentTokens payment_tokens;

  // Act
  payment_tokens.SetTokens({});

  // Assert
  EXPECT_TRUE(payment_tokens.IsEmpty());
}

TEST_F(HnsAdsPaymentTokensTest, AddTokens) {
  // Arrange
  const PaymentTokenList tokens = BuildPaymentTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  PaymentTokens payment_tokens;
  payment_tokens.SetTokens({tokens.at(0)});

  // Act
  payment_tokens.AddTokens({tokens.at(1)});

  // Assert
  EXPECT_EQ(2U, payment_tokens.Count());
}

TEST_F(HnsAdsPaymentTokensTest, AddEmptyTokens) {
  // Arrange
  PaymentTokens payment_tokens;

  // Act
  payment_tokens.AddTokens({});

  // Assert
  EXPECT_TRUE(payment_tokens.IsEmpty());
}

TEST_F(HnsAdsPaymentTokensTest, DoNotAddDuplicateTokens) {
  // Arrange
  const PaymentTokenInfo payment_token = BuildPaymentTokenForTesting();

  PaymentTokens payment_tokens;
  payment_tokens.AddTokens({payment_token});

  // Act
  payment_tokens.AddTokens({payment_token});

  // Assert
  EXPECT_EQ(1U, payment_tokens.Count());
}

TEST_F(HnsAdsPaymentTokensTest, RemoveToken) {
  // Arrange
  const PaymentTokenList tokens = BuildPaymentTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, tokens.size());

  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(tokens);

  const PaymentTokenInfo& token_1 = tokens.at(0);
  const PaymentTokenInfo& token_2 = tokens.at(1);

  // Act
  payment_tokens.RemoveToken(token_2);

  // Assert
  const PaymentTokenList expected_tokens = {token_1};
  EXPECT_EQ(expected_tokens, payment_tokens.GetAllTokens());
}

TEST_F(HnsAdsPaymentTokensTest, RemoveTokens) {
  // Arrange
  const PaymentTokenList tokens = BuildPaymentTokensForTesting(/*count*/ 3);
  ASSERT_EQ(3U, tokens.size());

  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(tokens);

  const PaymentTokenInfo& token_1 = tokens.at(0);
  const PaymentTokenInfo& token_2 = tokens.at(1);
  const PaymentTokenInfo& token_3 = tokens.at(2);

  // Act
  payment_tokens.RemoveTokens({token_1, token_3});

  // Assert
  const PaymentTokenList expected_tokens = {token_2};
  EXPECT_EQ(expected_tokens, payment_tokens.GetAllTokens());
}

TEST_F(HnsAdsPaymentTokensTest, RemoveAllTokens) {
  // Arrange
  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(BuildPaymentTokensForTesting(/*count*/ 2));

  // Act
  payment_tokens.RemoveAllTokens();

  // Assert
  EXPECT_TRUE(payment_tokens.IsEmpty());
}

TEST_F(HnsAdsPaymentTokensTest, TokenDoesExist) {
  // Arrange
  const PaymentTokenInfo payment_token = BuildPaymentTokenForTesting();

  PaymentTokens payment_tokens;
  payment_tokens.SetTokens({payment_token});

  // Act

  // Assert
  EXPECT_TRUE(payment_tokens.TokenExists(payment_token));
}

TEST_F(HnsAdsPaymentTokensTest, TokenDoesNotExist) {
  // Arrange
  PaymentTokens payment_tokens;

  // Act

  // Assert
  EXPECT_FALSE(payment_tokens.TokenExists(BuildPaymentTokenForTesting()));
}

TEST_F(HnsAdsPaymentTokensTest, Count) {
  // Arrange
  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(BuildPaymentTokensForTesting(/*count*/ 3));

  // Act

  // Assert
  EXPECT_EQ(3U, payment_tokens.Count());
}

TEST_F(HnsAdsPaymentTokensTest, IsEmpty) {
  // Arrange
  const PaymentTokens payment_tokens;

  // Act

  // Assert
  EXPECT_TRUE(payment_tokens.IsEmpty());
}

TEST_F(HnsAdsPaymentTokensTest, IsNotEmpty) {
  // Arrange
  PaymentTokens payment_tokens;
  payment_tokens.SetTokens(BuildPaymentTokensForTesting(/*count*/ 1));

  // Act

  // Assert
  EXPECT_FALSE(payment_tokens.IsEmpty());
}

}  // namespace hns_ads
