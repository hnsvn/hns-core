/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/url_request_builders/create_non_reward_confirmation_url_request_builder.h"

#include "hns/components/hns_ads/core/internal/account/confirmations/non_reward/non_reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kExpectedUrl[] =
    "https://anonymous.ads.hnssoftware.com/v3/confirmation/"
    "8b742869-6e4a-490c-ac31-31b49130098a";
constexpr char kExpectedContent[] =
    R"({"creativeInstanceId":"546fe7b0-5047-4f28-a11c-81f14edcf0f6","transactionId":"8b742869-6e4a-490c-ac31-31b49130098a","type":"view"})";

}  // namespace

class HnsAdsCreateNonRewardConfirmationUrlRequestBuilderTest
    : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    DisableHnsRewardsForTesting();
  }
};

TEST_F(HnsAdsCreateNonRewardConfirmationUrlRequestBuilderTest, BuildUrl) {
  // Arrange
  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  CreateNonRewardConfirmationUrlRequestBuilder url_request_builder(
      *confirmation);

  // Act
  const mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();

  // Assert
  mojom::UrlRequestInfoPtr expected_url_request = mojom::UrlRequestInfo::New();
  expected_url_request->url = GURL(kExpectedUrl);
  expected_url_request->headers = {"accept: application/json"};
  expected_url_request->content = kExpectedContent;
  expected_url_request->content_type = "application/json";
  expected_url_request->method = mojom::UrlRequestMethodType::kPost;

  EXPECT_EQ(url_request, expected_url_request);
}

}  // namespace hns_ads
