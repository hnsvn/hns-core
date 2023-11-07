/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/statement/next_payment_date_util.h"

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/account/statement/statement_feature.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNextPaymentDateUtilTest : public UnitTestBase {};

TEST_F(HnsAdsNextPaymentDateUtilTest,
       TimeNowIsBeforeNextPaymentDayWithReconciledTransactionsLastMonth) {
  // Arrange
  base::FieldTrialParams params;
  params["next_payment_day"] = "5";
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kAccountStatementFeature, params);

  AdvanceClockTo(TimeFromString("1 January 2020", /*is_local*/ false));

  TransactionList transactions;
  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  AdvanceClockTo(TimeFromString("1 February 2020", /*is_local*/ false));

  const base::Time next_token_redemption_at =
      TimeFromString("5 February 2020", /*is_local*/ false);

  // Act
  const base::Time next_payment_date =
      CalculateNextPaymentDate(next_token_redemption_at, transactions);

  // Assert
  const base::Time expected_next_payment_date =
      TimeFromString("5 February 2020 23:59:59.999", /*is_local*/ false);
  EXPECT_EQ(expected_next_payment_date, next_payment_date);
}

TEST_F(HnsAdsNextPaymentDateUtilTest,
       TimeNowIsBeforeNextPaymentDayWithNoReconciledTransactionsLastMonth) {
  // Arrange
  base::FieldTrialParams params;
  params["next_payment_day"] = "5";
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kAccountStatementFeature, params);

  AdvanceClockTo(TimeFromString("1 February 2020", /*is_local*/ false));

  const TransactionList transactions;

  const base::Time next_token_redemption_at =
      TimeFromString("5 February 2020", /*is_local*/ false);

  // Act
  const base::Time next_payment_date =
      CalculateNextPaymentDate(next_token_redemption_at, transactions);

  // Assert
  const base::Time expected_next_payment_date =
      TimeFromString("5 March 2020 23:59:59.999", /*is_local*/ false);
  EXPECT_EQ(expected_next_payment_date, next_payment_date);
}

TEST_F(HnsAdsNextPaymentDateUtilTest,
       TimeNowIsAfterNextPaymentDayWithReconciledTransactionsThisMonth) {
  // Arrange
  base::FieldTrialParams params;
  params["next_payment_day"] = "5";
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kAccountStatementFeature, params);

  AdvanceClockTo(TimeFromString("31 January 2020", /*is_local*/ false));

  TransactionList transactions;
  const TransactionInfo transaction = BuildTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed, /*reconciled_at*/ Now(),
      /*should_use_random_uuids*/ true);
  transactions.push_back(transaction);

  const base::Time next_token_redemption_at =
      TimeFromString("5 February 2020", /*is_local*/ false);

  // Act
  const base::Time next_payment_date =
      CalculateNextPaymentDate(next_token_redemption_at, transactions);

  // Assert
  const base::Time expected_next_payment_date =
      TimeFromString("5 February 2020 23:59:59.999", /*is_local*/ false);
  EXPECT_EQ(expected_next_payment_date, next_payment_date);
}

TEST_F(
    HnsAdsNextPaymentDateUtilTest,
    TimeNowIsAfterNextPaymentDayWhenNextTokenRedemptionDateIsThisMonthAndNoReconciledTransactionsThisMonth) {  // NOLINT
  // Arrange
  base::FieldTrialParams params;
  params["next_payment_day"] = "5";
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kAccountStatementFeature, params);

  AdvanceClockTo(TimeFromString("11 January 2020", /*is_local*/ false));

  const TransactionList transactions;

  const base::Time next_token_redemption_at =
      TimeFromString("31 January 2020", /*is_local*/ false);

  // Act
  const base::Time next_payment_date =
      CalculateNextPaymentDate(next_token_redemption_at, transactions);

  // Assert
  const base::Time expected_next_payment_date =
      TimeFromString("5 February 2020 23:59:59.999", /*is_local*/ false);
  EXPECT_EQ(expected_next_payment_date, next_payment_date);
}

TEST_F(
    HnsAdsNextPaymentDateUtilTest,
    TimeNowIsAfterNextPaymentDayWhenNextTokenRedemptionDateIsNextMonthAndNoReconciledTransactionsThisMonth) {  // NOLINT
  // Arrange
  base::FieldTrialParams params;
  params["next_payment_day"] = "5";
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kAccountStatementFeature, params);

  AdvanceClockTo(TimeFromString("31 January 2020", /*is_local*/ false));

  const TransactionList transactions;

  const base::Time next_token_redemption_at =
      TimeFromString("5 February 2020", /*is_local*/ false);

  // Act
  const base::Time next_payment_date =
      CalculateNextPaymentDate(next_token_redemption_at, transactions);

  // Assert
  const base::Time expected_next_payment_date =
      TimeFromString("5 March 2020 23:59:59.999", /*is_local*/ false);
  EXPECT_EQ(expected_next_payment_date, next_payment_date);
}

}  // namespace hns_ads
