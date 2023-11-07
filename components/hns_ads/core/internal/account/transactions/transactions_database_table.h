/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_DATABASE_TABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_DATABASE_TABLE_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/database/database_table_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads::database::table {

using GetTransactionsCallback =
    base::OnceCallback<void(bool success, const TransactionList& transactions)>;

class Transactions final : public TableInterface {
 public:
  void Save(const TransactionList& transactions, ResultCallback callback);

  void GetAll(GetTransactionsCallback callback) const;
  void GetForDateRange(base::Time from_time,
                       base::Time to_time,
                       GetTransactionsCallback callback) const;

  void Update(const PaymentTokenList& payment_tokens,
              ResultCallback callback) const;

  void Delete(ResultCallback callback) const;

  std::string GetTableName() const override;

  void Create(mojom::DBTransactionInfo* transaction) override;
  void Migrate(mojom::DBTransactionInfo* transaction, int to_version) override;

 private:
  void InsertOrUpdate(mojom::DBTransactionInfo* transaction,
                      const TransactionList& transactions);

  std::string BuildInsertOrUpdateSql(mojom::DBCommandInfo* command,
                                     const TransactionList& transactions) const;
};

}  // namespace hns_ads::database::table

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_DATABASE_TABLE_H_
