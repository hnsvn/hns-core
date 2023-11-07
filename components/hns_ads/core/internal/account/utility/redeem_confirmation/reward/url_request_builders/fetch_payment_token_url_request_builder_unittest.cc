/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/url_request_builders/fetch_payment_token_url_request_builder.h"

#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_mock.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsFetchPaymentTokenUrlRequestBuilderTest : public UnitTestBase {
 protected:
  ::testing::NiceMock<TokenGeneratorMock> token_generator_mock_;
};

TEST_F(HnsAdsFetchPaymentTokenUrlRequestBuilderTest, BuildUrl) {
  // Arrange
  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  FetchPaymentTokenUrlRequestBuilder url_request_builder(*confirmation);

  // Act
  const mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();

  // Assert
  mojom::UrlRequestInfoPtr expected_url_request = mojom::UrlRequestInfo::New();
  expected_url_request->url = GURL(
      "https://anonymous.ads.hnssoftware.com/v3/confirmation/"
      "8b742869-6e4a-490c-ac31-31b49130098a/paymentToken");
  expected_url_request->method = mojom::UrlRequestMethodType::kGet;

  EXPECT_EQ(expected_url_request, url_request);
}

}  // namespace hns_ads
