/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/user_data/redeem_payment_tokens_user_data_builder.h"

#include "base/test/mock_callback.h"
#include "base/test/values_test_util.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/user_data/build_user_data_callback.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRedeemPaymentTokensUserDataBuilderTest : public UnitTestBase {};

TEST_F(HnsAdsRedeemPaymentTokensUserDataBuilderTest, BuildUserData) {
  // Arrange

  // Assert
  const base::Value::Dict expected_user_data = base::test::ParseJsonDict(
      R"({"platform":"windows","totals":[{"ad_format":"ad_notification","view":2}]})");
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run(::testing::Eq(std::ref(expected_user_data))));

  // Act
  BuildRedeemPaymentTokensUserData(BuildPaymentTokensForTesting(/*count*/ 2),
                                   callback.Get());
}

}  // namespace hns_ads
