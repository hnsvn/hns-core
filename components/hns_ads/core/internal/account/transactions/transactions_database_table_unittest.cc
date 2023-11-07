/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/transactions/transactions_database_table.h"

#include <utility>

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_container_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::database::table {

namespace {

void ExpectTransactionsEq(const TransactionList expected_transactions) {
  base::MockCallback<GetTransactionsCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([&expected_transactions](const bool success,
                                         const TransactionList& transactions) {
        EXPECT_TRUE(success);
        EXPECT_TRUE(ContainersEq(expected_transactions, transactions));
      });

  const Transactions database_table;
  database_table.GetAll(callback.Get());
}

}  // namespace

class HnsAdsTransactionsDatabaseTableTest : public UnitTestBase {};

TEST_F(HnsAdsTransactionsDatabaseTableTest, SaveEmptyTransactions) {
  // Arrange

  // Act
  SaveTransactionsForTesting({});

  // Assert
  ExpectTransactionsEq({});
}

TEST_F(HnsAdsTransactionsDatabaseTableTest, SaveTransactions) {
  // Arrange
  TransactionList transactions;

  const TransactionInfo transaction_1 = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, DistantFuture(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_1);

  AdvanceClockBy(base::Days(5));

  const TransactionInfo transaction_2 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.03, ConfirmationType::kClicked,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_2);

  // Act
  SaveTransactionsForTesting(transactions);

  // Assert
  ExpectTransactionsEq(transactions);
}

TEST_F(HnsAdsTransactionsDatabaseTableTest, DoNotSaveDuplicateTransactions) {
  // Arrange
  TransactionList transactions;

  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  SaveTransactionsForTesting(transactions);

  // Act
  SaveTransactionsForTesting(transactions);

  // Assert
  ExpectTransactionsEq(transactions);
}

TEST_F(HnsAdsTransactionsDatabaseTableTest, GetTransactionsForDateRange) {
  // Arrange
  TransactionList transactions;

  const TransactionInfo transaction_1 = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, DistantFuture(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_1);

  AdvanceClockBy(base::Days(5));

  const TransactionInfo transaction_2 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.03, ConfirmationType::kClicked,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_2);

  SaveTransactionsForTesting(transactions);

  // Assert
  const TransactionList expected_transactions = {transaction_2};
  base::MockCallback<GetTransactionsCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([&expected_transactions](const bool success,
                                         const TransactionList& transactions) {
        EXPECT_TRUE(success);
        EXPECT_TRUE(ContainersEq(expected_transactions, transactions));
      });

  // Act
  const Transactions database_table;
  database_table.GetForDateRange(Now(), DistantFuture(), callback.Get());
}

TEST_F(HnsAdsTransactionsDatabaseTableTest, UpdateTransactions) {
  // Arrange
  TransactionList transactions;

  const TransactionInfo transaction_1 = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, DistantFuture(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_1);

  TransactionInfo transaction_2 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.03, ConfirmationType::kClicked,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_2);

  SaveTransactionsForTesting(transactions);

  PaymentTokenList payment_tokens;
  PaymentTokenInfo payment_token;
  payment_token.transaction_id = transaction_2.id;
  payment_tokens.push_back(payment_token);

  // Assert
  transaction_2.reconciled_at = Now();
  const TransactionList expected_transactions = {transaction_1, transaction_2};
  base::MockCallback<ResultCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([&expected_transactions](const bool success) {
        EXPECT_TRUE(success);
        ExpectTransactionsEq(expected_transactions);
      });

  // Act
  const Transactions database_table;
  database_table.Update(payment_tokens, callback.Get());
}

TEST_F(HnsAdsTransactionsDatabaseTableTest, DeleteTransactions) {
  // Arrange
  TransactionList transactions;

  const TransactionInfo transaction_1 = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, DistantFuture(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_1);

  const TransactionInfo transaction_2 = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.03, ConfirmationType::kClicked,
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction_2);

  SaveTransactionsForTesting(transactions);

  // Assert
  base::MockCallback<ResultCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](const bool success) {
    EXPECT_TRUE(success);
    ExpectTransactionsEq({});
  });

  // Act
  const Transactions database_table;
  database_table.Delete(callback.Get());
}

TEST_F(HnsAdsTransactionsDatabaseTableTest, TableName) {
  // Arrange
  const Transactions database_table;

  // Act

  // Assert
  EXPECT_EQ("transactions", database_table.GetTableName());
}

}  // namespace hns_ads::database::table
