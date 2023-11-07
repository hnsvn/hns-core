/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/deposits/non_cash_deposit.h"

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNonCashDepositTest : public UnitTestBase {};

TEST_F(HnsAdsNonCashDepositTest, GetValue) {
  // Arrange

  // Assert
  base::MockCallback<GetDepositCallback> callback;
  EXPECT_CALL(callback, Run(/*success*/ true, /*value*/ 0.0));

  // Act
  NonCashDeposit deposit;
  deposit.GetValue(kCreativeInstanceId, callback.Get());
}

}  // namespace hns_ads
