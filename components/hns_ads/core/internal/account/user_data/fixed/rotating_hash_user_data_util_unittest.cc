/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/rotating_hash_user_data_util.h"

#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRotatingHashUserDataUtilTest : public UnitTestBase {};

TEST_F(HnsAdsRotatingHashUserDataUtilTest,
       DoNotBuildRotatingHashIfMissingDeviceId) {
  // Arrange
  AdvanceClockTo(TimeFromString("2 June 2022 11:00", /*is_local*/ false));

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  // Act
  const absl::optional<std::string> rotating_hash =
      BuildRotatingHash(transaction);

  // Assert
  EXPECT_FALSE(rotating_hash);
}

TEST_F(HnsAdsRotatingHashUserDataUtilTest, BuildRotatingHash) {
  // Arrange
  MockDeviceId();

  AdvanceClockTo(TimeFromString("2 June 2022 11:00", /*is_local*/ false));

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  // Act
  const absl::optional<std::string> rotating_hash =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash);

  // Assert
  EXPECT_EQ("j9D7eKSoPLYNfxkG2Mx+SbgKJ9hcKg1QwDB8B5qxlpk=", rotating_hash);
}

TEST_F(HnsAdsRotatingHashUserDataUtilTest,
       BuildRotatingHashIfWithinSameHour) {
  // Arrange
  MockDeviceId();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  AdvanceClockTo(TimeFromString("2 June 2022 11:00", /*is_local*/ false));

  const absl::optional<std::string> rotating_hash_before =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash_before);

  // Act
  AdvanceClockBy(base::Hours(1) - base::Milliseconds(1));

  const absl::optional<std::string> rotating_hash_after =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash_after);

  // Assert
  EXPECT_EQ(rotating_hash_before, rotating_hash_after);
}

TEST_F(HnsAdsRotatingHashUserDataUtilTest,
       BuildRotatingHashForDifferentHours) {
  // Arrange
  MockDeviceId();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  AdvanceClockTo(TimeFromString("2 June 2022 11:00", /*is_local*/ false));

  const absl::optional<std::string> rotating_hash_before =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash_before);

  // Act
  AdvanceClockBy(base::Hours(1));

  const absl::optional<std::string> rotating_hash_after =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash_after);

  // Assert
  EXPECT_NE(rotating_hash_before, rotating_hash_after);
}

TEST_F(HnsAdsRotatingHashUserDataUtilTest,
       BuildRotatingHashForSameHourButDifferentDay) {
  // Arrange
  MockDeviceId();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  const absl::optional<std::string> rotating_hash_before =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash_before);

  // Act
  AdvanceClockBy(base::Days(1));

  const absl::optional<std::string> rotating_hash_after =
      BuildRotatingHash(transaction);
  EXPECT_TRUE(rotating_hash_after);

  // Assert
  EXPECT_NE(rotating_hash_before, rotating_hash_after);
}

}  // namespace hns_ads
