/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/queue/confirmation_queue_util.h"

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/non_reward/non_reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/user_data_builder/confirmation_user_data_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_mock.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_unittest_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionQueueUtilTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    MockConfirmationUserData();

    AdvanceClockTo(
        TimeFromString("November 18 2020 12:34:56.789", /*is_local*/ false));
  }

  ::testing::NiceMock<TokenGeneratorMock> token_generator_mock_;
};

TEST_F(HnsAdsConversionQueueUtilTest, AddRewardConfirmationQueueItem) {
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
  AddConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_EQ(confirmation, MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest,
       AddConversionRewardConfirmationQueueItem) {
  // Arrange
  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kViewed,
      /*is_verifiable*/ false, /*should_use_random_uuids*/ false, /*count*/ 1);

  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.0, ConfirmationType::kConversion,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  AddConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_EQ(confirmation, MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest,
       AddVerifiableConversionRewardConfirmationQueueItem) {
  // Arrange
  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kClicked,
      /*is_verifiable*/ true, /*should_use_random_uuids*/ false, /*count*/ 1);

  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.0, ConfirmationType::kConversion,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  AddConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_EQ(confirmation, MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest, AddNonRewardConfirmationQueueItem) {
  // Arrange
  DisableHnsRewardsForTesting();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  AddConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_EQ(confirmation, MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest,
       AddConversionNonRewardConfirmationQueueItem) {
  // Arrange
  DisableHnsRewardsForTesting();

  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kViewed,
      /*is_verifiable*/ false, /*should_use_random_uuids*/ false, /*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.0, ConfirmationType::kConversion,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  AddConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_EQ(confirmation, MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest,
       AddVerifiableConversionNonRewardConfirmationQueueItem) {
  // Arrange
  DisableHnsRewardsForTesting();

  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kClicked,
      /*is_verifiable*/ true, /*should_use_random_uuids*/ false, /*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.0, ConfirmationType::kConversion,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act
  AddConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_EQ(confirmation, MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest, RemoveConfirmationQueueItem) {
  // Arrange
  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  AddConfirmationQueueItem(*confirmation);
  ASSERT_TRUE(MaybeGetNextConfirmationQueueItem());

  // Act
  RemoveConfirmationQueueItem(*confirmation);

  // Assert
  EXPECT_FALSE(MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest, GetRewardConfirmationQueueItem) {
  // Arrange
  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  AddConfirmationQueueItem(*confirmation);

  // Act

  // Assert
  EXPECT_TRUE(MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest, GetNonRewardConfirmationQueueItem) {
  // Arrange
  DisableHnsRewardsForTesting();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction,
                                 /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  AddConfirmationQueueItem(*confirmation);

  // Act

  // Assert
  EXPECT_TRUE(MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest,
       DoNotGetConfirmationQueueItemIfQueueIsEmpty) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MaybeGetNextConfirmationQueueItem());
}

TEST_F(HnsAdsConversionQueueUtilTest, RebuildRewardConfirmationQueueItem) {
  // Arrange
  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  base::MockCallback<RebuildConfirmationQueueItemCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([&confirmation](const ConfirmationInfo& rebuilt_confirmation) {
        EXPECT_NE(confirmation, rebuilt_confirmation);
      });

  // Act
  RebuildConfirmationQueueItem(*confirmation, callback.Get());
}

TEST_F(HnsAdsConversionQueueUtilTest, RebuildNonRewardConfirmationQueueItem) {
  // Arrange
  DisableHnsRewardsForTesting();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  const absl::optional<ConfirmationInfo> confirmation =
      BuildNonRewardConfirmation(transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Assert
  base::MockCallback<RebuildConfirmationQueueItemCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([&confirmation](const ConfirmationInfo& rebuilt_confirmation) {
        EXPECT_EQ(confirmation, rebuilt_confirmation);
      });

  // Act
  RebuildConfirmationQueueItem(*confirmation, callback.Get());
}

}  // namespace hns_ads
