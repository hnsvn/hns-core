/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/redeem_non_reward_confirmation.h"

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/non_reward/non_reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/redeem_non_reward_confirmation_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/url_request_builders/create_non_reward_confirmation_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_delegate_mock.h"
#include "hns/components/hns_ads/core/internal/common/net/http/http_status_code.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRedeemNonRewardConfirmationTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    DisableHnsRewardsForTesting();
  }

  ::testing::NiceMock<RedeemConfirmationDelegateMock>
      redeem_confirmation_delegate_mock_;
  base::WeakPtrFactory<RedeemConfirmationDelegateMock>
      confirmation_delegate_weak_factory_{&redeem_confirmation_delegate_mock_};
};

TEST_F(HnsAdsRedeemNonRewardConfirmationTest, Redeem) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::kHttpImATeapot,
         BuildCreateNonRewardConfirmationUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  EXPECT_CALL(redeem_confirmation_delegate_mock_,
              OnDidRedeemConfirmation(*confirmation));

  EXPECT_CALL(redeem_confirmation_delegate_mock_, OnFailedToRedeemConfirmation)
      .Times(0);

  // Act
  RedeemNonRewardConfirmation::CreateAndRedeem(
      confirmation_delegate_weak_factory_.GetWeakPtr(), *confirmation);
}

TEST_F(HnsAdsRedeemNonRewardConfirmationTest,
       DoNotRetryRedeemingForHttpBadRequestResponse) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::HTTP_BAD_REQUEST,
         /*response_body*/ net::GetHttpReasonPhrase(net::HTTP_BAD_REQUEST)}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  EXPECT_CALL(redeem_confirmation_delegate_mock_, OnDidRedeemConfirmation)
      .Times(0);

  EXPECT_CALL(
      redeem_confirmation_delegate_mock_,
      OnFailedToRedeemConfirmation(*confirmation, /*should_retry*/ false));

  // Act
  RedeemNonRewardConfirmation::CreateAndRedeem(
      confirmation_delegate_weak_factory_.GetWeakPtr(), *confirmation);
}

TEST_F(HnsAdsRedeemNonRewardConfirmationTest,
       DoNotRetryRedeemingForHttpConflictResponse) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::HTTP_CONFLICT,
         /*response_body*/ net::GetHttpReasonPhrase(net::HTTP_CONFLICT)}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  EXPECT_CALL(redeem_confirmation_delegate_mock_, OnDidRedeemConfirmation)
      .Times(0);

  EXPECT_CALL(
      redeem_confirmation_delegate_mock_,
      OnFailedToRedeemConfirmation(*confirmation, /*should_retry*/ false));

  // Act
  RedeemNonRewardConfirmation::CreateAndRedeem(
      confirmation_delegate_weak_factory_.GetWeakPtr(), *confirmation);
}

TEST_F(HnsAdsRedeemNonRewardConfirmationTest,
       DoNotRetryReemingForHttpCreatedResponse) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::HTTP_CREATED,
         /*response_body*/ net::GetHttpReasonPhrase(net::HTTP_CREATED)}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  EXPECT_CALL(redeem_confirmation_delegate_mock_, OnDidRedeemConfirmation)
      .Times(0);

  EXPECT_CALL(
      redeem_confirmation_delegate_mock_,
      OnFailedToRedeemConfirmation(*confirmation, /*should_retry*/ false));

  // Act
  RedeemNonRewardConfirmation::CreateAndRedeem(
      confirmation_delegate_weak_factory_.GetWeakPtr(), *confirmation);
}

TEST_F(HnsAdsRedeemNonRewardConfirmationTest, RetryRedeeming) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::HTTP_INTERNAL_SERVER_ERROR,
         /*response_body*/ net::GetHttpReasonPhrase(
             net::HTTP_INTERNAL_SERVER_ERROR)}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  EXPECT_CALL(redeem_confirmation_delegate_mock_, OnDidRedeemConfirmation)
      .Times(0);

  EXPECT_CALL(
      redeem_confirmation_delegate_mock_,
      OnFailedToRedeemConfirmation(*confirmation, /*should_retry*/ true));

  // Act
  RedeemNonRewardConfirmation::CreateAndRedeem(
      confirmation_delegate_weak_factory_.GetWeakPtr(), *confirmation);
}

}  // namespace hns_ads
