/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/redeem_payment_tokens_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRedeemPaymentTokensUtilTest : public UnitTestBase {};

TEST_F(HnsAdsRedeemPaymentTokensUtilTest, SetNextTokenRedemptionAt) {
  // Arrange
  SetNextTokenRedemptionAt(DistantFuture());

  // Act

  // Assert
  EXPECT_FALSE(
      ads_client_mock_.GetTimePref(prefs::kNextTokenRedemptionAt).is_null());
}

TEST_F(HnsAdsRedeemPaymentTokensUtilTest, ScheduleNextTokenRedemptionAt) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ScheduleNextTokenRedemptionAt().is_null());
}

TEST_F(HnsAdsRedeemPaymentTokensUtilTest,
       CalculateDelayBeforeRedeemingTokens) {
  // Arrange
  SetNextTokenRedemptionAt(Now() + base::Days(1));

  // Act

  // Assert
  EXPECT_EQ(base::Days(1), CalculateDelayBeforeRedeemingTokens());
}

TEST_F(HnsAdsRedeemPaymentTokensUtilTest,
       CalculateDelayBeforeRedeemingTokensIfHasNotPreviouslyRedeemedTokens) {
  // Arrange
  SetNextTokenRedemptionAt({});

  // Act

  // Assert
  EXPECT_FALSE(ScheduleNextTokenRedemptionAt().is_null());
}

TEST_F(HnsAdsRedeemPaymentTokensUtilTest,
       CalculateDelayBeforeRedeemingTokensIfShouldHaveRedeemedTokensInThePast) {
  // Arrange
  SetNextTokenRedemptionAt(DistantPast());

  // Act
  EXPECT_EQ(base::Minutes(1), CalculateDelayBeforeRedeemingTokens());
}

TEST_F(HnsAdsRedeemPaymentTokensUtilTest,
       CalculateMinimumDelayBeforeRedeemingTokens) {
  // Arrange
  SetNextTokenRedemptionAt(Now() + base::Milliseconds(1));

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(1), CalculateDelayBeforeRedeemingTokens());
}

}  // namespace hns_ads
