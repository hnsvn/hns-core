/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_util.h"

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPaymentTokenUtilTest : public UnitTestBase {};

TEST_F(HnsAdsPaymentTokenUtilTest, GetPaymentToken) {
  // Arrange
  const PaymentTokenList payment_tokens =
      SetPaymentTokensForTesting(/*count*/ 2);

  // Act

  // Assert
  EXPECT_EQ(payment_tokens.front(), MaybeGetPaymentToken());
}

TEST_F(HnsAdsPaymentTokenUtilTest, DoNotGetPaymentToken) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MaybeGetPaymentToken());
}

TEST_F(HnsAdsPaymentTokenUtilTest, AddPaymentTokens) {
  // Arrange
  const PaymentTokenList payment_tokens =
      BuildPaymentTokensForTesting(/*count*/ 2);
  ASSERT_EQ(2U, payment_tokens.size());

  const PaymentTokenInfo& token_1 = payment_tokens.at(0);
  const PaymentTokenInfo& token_2 = payment_tokens.at(1);

  GetPaymentTokensForTesting().SetTokens({token_1});

  // Act
  AddPaymentTokens({token_2});

  // Assert
  const PaymentTokenList expected_tokens = {token_1, token_2};
  EXPECT_EQ(expected_tokens, GetAllPaymentTokens());
}

TEST_F(HnsAdsPaymentTokenUtilTest, RemovePaymentToken) {
  // Arrange
  const PaymentTokenList payment_tokens =
      BuildPaymentTokensForTesting(/*count*/ 3);
  ASSERT_EQ(3U, payment_tokens.size());

  const PaymentTokenInfo& token_1 = payment_tokens.at(0);
  const PaymentTokenInfo& token_2 = payment_tokens.at(1);
  const PaymentTokenInfo& token_3 = payment_tokens.at(2);

  GetPaymentTokensForTesting().SetTokens(payment_tokens);

  // Act
  RemovePaymentToken(token_2);

  // Assert
  const PaymentTokenList expected_tokens = {token_1, token_3};
  EXPECT_EQ(expected_tokens, GetAllPaymentTokens());
}

TEST_F(HnsAdsPaymentTokenUtilTest, PaymentTokenCount) {
  // Arrange
  SetPaymentTokensForTesting(/*count*/ 3);

  // Act

  // Assert
  EXPECT_EQ(3U, PaymentTokenCount());
}

}  // namespace hns_ads
