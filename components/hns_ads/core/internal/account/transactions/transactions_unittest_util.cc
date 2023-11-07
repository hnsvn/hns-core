/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/time/time.h"
#include "base/uuid.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_database_table.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

size_t GetTransactionCountForTesting() {
  size_t count = 0;

  GetTransactionsForDateRange(
      DistantPast(), DistantFuture(),
      base::BindOnce(
          [](size_t* count, const bool success,
             const TransactionList& transactions) mutable {
            CHECK(success);
            *count = transactions.size();
          },
          &count));

  return count;
}

void SaveTransactionsForTesting(const TransactionList& transactions) {
  database::table::Transactions database_table;
  database_table.Save(
      transactions, base::BindOnce([](const bool success) { CHECK(success); }));
}

TransactionInfo BuildTransactionForTesting(
    const double value,
    const ConfirmationType& confirmation_type,
    const base::Time reconciled_at,
    const bool should_use_random_uuids) {
  TransactionInfo transaction;

  transaction.id = should_use_random_uuids
                       ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                       : kTransactionId;
  transaction.created_at = Now();
  transaction.creative_instance_id =
      should_use_random_uuids
          ? base::Uuid::GenerateRandomV4().AsLowercaseString()
          : kCreativeInstanceId;
  transaction.value = value;
  transaction.segment = "untargeted";
  transaction.ad_type = AdType::kNotificationAd;
  transaction.confirmation_type = confirmation_type;
  transaction.reconciled_at = reconciled_at;

  return transaction;
}

TransactionInfo BuildUnreconciledTransactionForTesting(
    const double value,
    const ConfirmationType& confirmation_type,
    const bool should_use_random_uuids) {
  return BuildTransactionForTesting(
      value, confirmation_type, /*reconciled_at*/ {}, should_use_random_uuids);
}

}  // namespace hns_ads
