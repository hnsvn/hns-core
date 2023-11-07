/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/refill_confirmation_tokens_util.h"

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/tokens_feature.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRefillConfirmationTokensUtilTest : public UnitTestBase {};

TEST_F(HnsAdsRefillConfirmationTokensUtilTest,
       ShouldRefillConfirmationTokens) {
  // Arrange
  const int count = kMinConfirmationTokens.Get() - 1;
  SetConfirmationTokensForTesting(count);

  // Act

  // Assert
  EXPECT_TRUE(ShouldRefillConfirmationTokens());
}

TEST_F(HnsAdsRefillConfirmationTokensUtilTest,
       ShouldNotRefillConfirmationTokens) {
  // Arrange
  const int count = kMinConfirmationTokens.Get();
  SetConfirmationTokensForTesting(count);

  // Act

  // Assert
  EXPECT_FALSE(ShouldRefillConfirmationTokens());
}

TEST_F(HnsAdsRefillConfirmationTokensUtilTest,
       CalculateAmountOfConfirmationTokensToRefill) {
  // Arrange
  SetConfirmationTokensForTesting(/*count*/ 10);

  // Act

  // Assert
  EXPECT_EQ(kMaxConfirmationTokens.Get() - 10,
            CalculateAmountOfConfirmationTokensToRefill());
}

}  // namespace hns_ads
