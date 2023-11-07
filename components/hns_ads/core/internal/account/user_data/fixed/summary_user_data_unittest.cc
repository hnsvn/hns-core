/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/summary_user_data.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsSummaryUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsSummaryUserDataTest, BuildSummaryUserDataForRewardsUser) {
  // Arrange
  PaymentTokenList payment_tokens;

  const PaymentTokenInfo payment_token_1 = BuildPaymentTokenForTesting(
      ConfirmationType::kViewed, AdType::kNotificationAd);
  payment_tokens.push_back(payment_token_1);

  const PaymentTokenInfo payment_token_2 = BuildPaymentTokenForTesting(
      ConfirmationType::kViewed, AdType::kNotificationAd);
  payment_tokens.push_back(payment_token_2);

  const PaymentTokenInfo payment_token_3 = BuildPaymentTokenForTesting(
      ConfirmationType::kClicked, AdType::kNotificationAd);
  payment_tokens.push_back(payment_token_3);

  const PaymentTokenInfo payment_token_4 = BuildPaymentTokenForTesting(
      ConfirmationType::kViewed, AdType::kInlineContentAd);
  payment_tokens.push_back(payment_token_4);

  // Act

  // Assert
  EXPECT_EQ(
      base::test::ParseJsonDict(
          R"({"totals":[{"ad_format":"ad_notification","click":1,"view":2},{"ad_format":"inline_content_ad","view":1}]})"),
      BuildSummaryUserData(payment_tokens));
}

TEST_F(HnsAdsSummaryUserDataTest, BuildSummaryUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  const PaymentTokenList payment_tokens =
      BuildPaymentTokensForTesting(/*count*/ 3);

  // Act

  // Assert
  EXPECT_TRUE(BuildSummaryUserData(payment_tokens).empty());
}

TEST_F(HnsAdsSummaryUserDataTest, BuildSummaryUserDataIfNoPaymentTokens) {
  // Arrange
  const PaymentTokenList payment_tokens;

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"totals":[]})"),
            BuildSummaryUserData(payment_tokens));
}

}  // namespace hns_ads
