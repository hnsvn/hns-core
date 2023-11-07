/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/queue/confirmation_queue.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/non_reward/non_reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/queue/confirmation_queue_delegate.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_mock.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/redeem_non_reward_confirmation_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/url_request_builders/create_non_reward_confirmation_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/redeem_reward_confirmation_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/url_request_builders/create_reward_confirmation_url_request_builder_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/url_request_builders/create_reward_confirmation_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/url_request_builders/fetch_payment_token_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/net/http/http_status_code.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_url_response_alias.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "net/http/http_status_code.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConfirmationQueueTest : public ConfirmationQueueDelegate,
                                      public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    confirmation_queue_ = std::make_unique<ConfirmationQueue>();
    confirmation_queue_->SetDelegate(this);
  }

  void OnDidAddConfirmationToQueue(
      const ConfirmationInfo& confirmation) override {
    confirmation_ = confirmation;
    did_add_to_queue_ = true;
  }

  void OnWillProcessConfirmationQueue(const ConfirmationInfo& /*confirmation*/,
                                      base::Time process_at) override {
    // We should not set |confirmation_| because otherwise, this will be called
    // when the next item in the queue is processed.

    will_process_queue_at_ = process_at;
  }

  void OnDidProcessConfirmationQueue(
      const ConfirmationInfo& confirmation) override {
    confirmation_ = confirmation;
    did_process_queue_ = true;
  }

  void OnFailedToProcessConfirmationQueue(
      const ConfirmationInfo& /*confirmation*/) override {
    failed_to_process_queue_ = true;
  }

  void OnDidExhaustConfirmationQueue() override { did_exhaust_queue_ = true; }

  void ResetDelegate() {
    confirmation_.reset();
    did_add_to_queue_ = false;
    will_process_queue_at_.reset();
    did_process_queue_ = false;
    failed_to_process_queue_ = false;
    did_exhaust_queue_ = false;
  }

  ::testing::NiceMock<TokenGeneratorMock> token_generator_mock_;

  std::unique_ptr<ConfirmationQueue> confirmation_queue_;

  absl::optional<ConfirmationInfo> confirmation_;
  bool did_add_to_queue_ = false;
  absl::optional<base::Time> will_process_queue_at_;
  bool did_process_queue_ = false;
  bool failed_to_process_queue_ = false;
  bool did_exhaust_queue_ = false;
};

TEST_F(HnsAdsConfirmationQueueTest, AddRewardConfirmationToQueue) {
  // Arrange
  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  confirmation_queue_->Add(*confirmation);

  // Assert
  EXPECT_TRUE(did_add_to_queue_);
  EXPECT_TRUE(will_process_queue_at_);
  EXPECT_FALSE(did_process_queue_);
  EXPECT_FALSE(failed_to_process_queue_);
  EXPECT_FALSE(did_exhaust_queue_);
}

TEST_F(HnsAdsConfirmationQueueTest, AddNonRewardConfirmationToQueue) {
  // Arrange
  DisableHnsRewardsForTesting();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  confirmation_queue_->Add(*confirmation);

  // Assert
  EXPECT_TRUE(did_add_to_queue_);
  EXPECT_TRUE(will_process_queue_at_);
  EXPECT_FALSE(did_process_queue_);
  EXPECT_FALSE(failed_to_process_queue_);
  EXPECT_FALSE(did_exhaust_queue_);
}

TEST_F(HnsAdsConfirmationQueueTest, ProcessRewardConfirmationInQueue) {
  // Arrange
  BuildAndSetIssuersForTesting();

  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  const URLResponseMap url_responses = {
      {BuildCreateRewardConfirmationUrlPath(
           kTransactionId, kCreateRewardConfirmationCredential),
       {{net::HTTP_CREATED,
         BuildCreateRewardConfirmationUrlResponseBodyForTesting()}}},
      {BuildFetchPaymentTokenUrlPath(kTransactionId),
       {{net::HTTP_OK, BuildFetchPaymentTokenUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  confirmation_queue_->Add(*confirmation);

  ASSERT_TRUE(confirmation_);
  ASSERT_TRUE(did_add_to_queue_);
  ASSERT_TRUE(will_process_queue_at_);
  ASSERT_FALSE(did_process_queue_);

  // Act
  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(confirmation_);
  EXPECT_TRUE(did_process_queue_);
  EXPECT_FALSE(failed_to_process_queue_);
  EXPECT_TRUE(did_exhaust_queue_);
  EXPECT_FALSE(HasPendingTasks());
}

TEST_F(HnsAdsConfirmationQueueTest, ProcessNonRewardConfirmationQueue) {
  // Arrange
  DisableHnsRewardsForTesting();

  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::kHttpImATeapot,
         BuildCreateNonRewardConfirmationUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction,
                                 /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  confirmation_queue_->Add(*confirmation);

  ASSERT_TRUE(confirmation_);
  ASSERT_TRUE(did_add_to_queue_);
  ASSERT_TRUE(will_process_queue_at_);
  ASSERT_FALSE(did_process_queue_);

  // Act
  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(confirmation_);
  EXPECT_TRUE(did_process_queue_);
  EXPECT_FALSE(failed_to_process_queue_);
  EXPECT_TRUE(did_exhaust_queue_);
  EXPECT_FALSE(HasPendingTasks());
}

TEST_F(HnsAdsConfirmationQueueTest,
       ProcessMultipleRewardConfirmationsInQueue) {
  // Arrange
  BuildAndSetIssuersForTesting();

  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  const URLResponseMap url_responses = {
      {BuildCreateRewardConfirmationUrlPath(
           kTransactionId, kCreateRewardConfirmationCredential),
       {{net::HTTP_CREATED,
         BuildCreateRewardConfirmationUrlResponseBodyForTesting()}}},
      {BuildFetchPaymentTokenUrlPath(kTransactionId),
       {{net::HTTP_OK, BuildFetchPaymentTokenUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  SetConfirmationTokensForTesting(/*count*/ 2);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  // Act
  {
    const absl::optional<ConfirmationInfo> confirmation =
        BuildRewardConfirmation(&token_generator_mock_, transaction,
                                /*user_data*/ {});
    ASSERT_TRUE(confirmation);

    ScopedTimerDelaySetterForTesting scoped_setter(base::Seconds(7));
    confirmation_queue_->Add(*confirmation);

    ASSERT_TRUE(confirmation_);
    ASSERT_TRUE(did_add_to_queue_);
    ASSERT_EQ(Now() + base::Seconds(7), will_process_queue_at_);
    ASSERT_FALSE(did_process_queue_);

    ResetDelegate();
  }

  {
    const absl::optional<ConfirmationInfo> confirmation =
        BuildRewardConfirmation(&token_generator_mock_, transaction,
                                /*user_data*/ {});
    ASSERT_TRUE(confirmation);

    confirmation_queue_->Add(*confirmation);

    ASSERT_TRUE(confirmation_);
    ASSERT_TRUE(did_add_to_queue_);
    ASSERT_FALSE(will_process_queue_at_);
    ASSERT_FALSE(did_process_queue_);

    ResetDelegate();
  }

  ASSERT_EQ(1U, GetPendingTaskCount());
  ASSERT_FALSE(did_exhaust_queue_);

  ScopedTimerDelaySetterForTesting scoped_setter(base::Seconds(21));
  FastForwardClockToNextPendingTask();

  ASSERT_TRUE(did_process_queue_);
  ASSERT_FALSE(failed_to_process_queue_);
  ASSERT_FALSE(did_exhaust_queue_);
  ASSERT_EQ(1U, GetPendingTaskCount());

  // Assert
  EXPECT_EQ(Now() + base::Seconds(21), will_process_queue_at_);
}

TEST_F(HnsAdsConfirmationQueueTest,
       ProcessMultipleNonRewardConfirmationsInQueue) {
  // Arrange
  DisableHnsRewardsForTesting();

  const URLResponseMap url_responses = {
      {BuildCreateNonRewardConfirmationUrlPath(kTransactionId),
       {{net::kHttpImATeapot,
         BuildCreateNonRewardConfirmationUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  // Act
  {
    const absl::optional<ConfirmationInfo> confirmation =
        BuildNonRewardConfirmation(transaction,
                                   /*user_data*/ {});
    ASSERT_TRUE(confirmation);

    ScopedTimerDelaySetterForTesting scoped_setter(base::Seconds(7));
    confirmation_queue_->Add(*confirmation);

    ASSERT_TRUE(confirmation_);
    ASSERT_TRUE(did_add_to_queue_);
    ASSERT_EQ(Now() + base::Seconds(7), will_process_queue_at_);
    ASSERT_FALSE(did_process_queue_);

    ResetDelegate();
  }

  {
    const absl::optional<ConfirmationInfo> confirmation =
        BuildNonRewardConfirmation(transaction,
                                   /*user_data*/ {});
    ASSERT_TRUE(confirmation);

    confirmation_queue_->Add(*confirmation);

    ASSERT_TRUE(confirmation_);
    ASSERT_TRUE(did_add_to_queue_);
    ASSERT_FALSE(will_process_queue_at_);
    ASSERT_FALSE(did_process_queue_);

    ResetDelegate();
  }

  ASSERT_EQ(1U, GetPendingTaskCount());
  ASSERT_FALSE(did_exhaust_queue_);

  ScopedTimerDelaySetterForTesting scoped_setter(base::Seconds(21));
  FastForwardClockToNextPendingTask();

  ASSERT_TRUE(did_process_queue_);
  ASSERT_FALSE(failed_to_process_queue_);
  ASSERT_FALSE(did_exhaust_queue_);
  ASSERT_EQ(1U, GetPendingTaskCount());

  // Assert
  EXPECT_EQ(Now() + base::Seconds(21), will_process_queue_at_);
}

}  // namespace hns_ads
