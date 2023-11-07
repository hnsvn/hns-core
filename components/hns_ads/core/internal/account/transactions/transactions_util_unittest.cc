/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/transactions/transactions_util.h"

#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTransactionsUtilTest : public UnitTestBase {};

TEST_F(HnsAdsTransactionsUtilTest, GetTransactionsForDateRange) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  TransactionList transactions;

  const TransactionInfo transaction_1 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_1);

  AdvanceClockTo(TimeFromString("25 December 2020", /*is_local*/ true));

  const TransactionInfo transaction_2 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.03, ConfirmationType::kClicked,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_2);

  // Act
  const TransactionList transactions_for_date_range =
      GetTransactionsForDateRange(transactions, Now(), DistantFuture());

  // Assert
  const TransactionList expected_transactions_for_date_range = {transaction_2};
  EXPECT_EQ(expected_transactions_for_date_range, transactions_for_date_range);
}

TEST_F(HnsAdsTransactionsUtilTest, DoNotGetTransactionsForDateRange) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  TransactionList transactions;

  const TransactionInfo transaction_1 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_1);

  const TransactionInfo transaction_2 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.03, ConfirmationType::kClicked,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_2);

  AdvanceClockTo(TimeFromString("25 December 2020", /*is_local*/ true));

  // Act
  const TransactionList transactions_for_date_range =
      GetTransactionsForDateRange(transactions, Now(), DistantFuture());

  // Assert
  EXPECT_TRUE(transactions_for_date_range.empty());
}

}  // namespace hns_ads
