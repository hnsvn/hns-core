/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

using AddTransactionCallback =
    base::OnceCallback<void(bool success, const TransactionInfo& transaction)>;

using GetTransactionsCallback =
    base::OnceCallback<void(bool success, const TransactionList& transactions)>;

using RemoveAllTransactionsCallback = base::OnceCallback<void(bool success)>;

class AdType;
class ConfirmationType;

TransactionInfo BuildTransaction(const std::string& creative_instance_id,
                                 const std::string& segment,
                                 double value,
                                 const AdType& ad_type,
                                 const ConfirmationType& confirmation_type);

TransactionInfo AddTransaction(const std::string& creative_instance_id,
                               const std::string& segment,
                               double value,
                               const AdType& ad_type,
                               const ConfirmationType& confirmation_type,
                               AddTransactionCallback callback);

void GetTransactionsForDateRange(base::Time from_time,
                                 base::Time to_time,
                                 GetTransactionsCallback callback);

void RemoveAllTransactions(RemoveAllTransactionsCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_H_
