/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/transactions/reconciled_transactions_util.h"

#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsReconciledTransactionsUtilTest : public UnitTestBase {};

TEST_F(HnsAdsReconciledTransactionsUtilTest,
       DidReconcileTransactionsThisMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  TransactionList transactions;
  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  // Act

  // Assert
  EXPECT_TRUE(DidReconcileTransactionsThisMonth(transactions));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest,
       DoesNotHaveReconciledTransactionsForThisMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  TransactionList transactions;
  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  AdvanceClockTo(TimeFromString("25 December 2020", /*is_local*/ true));

  // Act

  // Assert
  EXPECT_FALSE(DidReconcileTransactionsThisMonth(transactions));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest,
       DidReconcileTransactionsLastMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  TransactionList transactions;
  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  AdvanceClockTo(TimeFromString("25 December 2020", /*is_local*/ true));

  // Act

  // Assert
  EXPECT_TRUE(DidReconcileTransactionsLastMonth(transactions));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest,
       DoesNotHaveReconciledTransactionsForPreviousMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  TransactionList transactions;
  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  // Act

  // Assert
  EXPECT_FALSE(DidReconcileTransactionsLastMonth(transactions));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest, DidReconcileTransaction) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_TRUE(DidReconcileTransaction(transaction));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest, WasTransactionNotReconciled) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_FALSE(DidReconcileTransaction(transaction));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest,
       DidReconcileTransactionWithinDateRange) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_TRUE(DidReconcileTransactionWithinDateRange(transaction, DistantPast(),
                                                     Now()));
}

TEST_F(HnsAdsReconciledTransactionsUtilTest,
       HasTransactionNotReconciledForDateRange) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020", /*is_local*/ true));

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_FALSE(DidReconcileTransactionWithinDateRange(
      transaction, Now() + base::Milliseconds(1), DistantFuture()));
}

}  // namespace hns_ads
