/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_UNITTEST_UTIL_H_

#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

class ConfirmationType;

size_t GetTransactionCountForTesting();

void SaveTransactionsForTesting(const TransactionList& transactions);

TransactionInfo BuildTransactionForTesting(
    double value,
    const ConfirmationType& confirmation_type,
    base::Time reconciled_at,
    bool should_use_random_uuids);
TransactionInfo BuildUnreconciledTransactionForTesting(
    double value,
    const ConfirmationType& confirmation_type,
    bool should_use_random_uuids);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_UNITTEST_UTIL_H_
