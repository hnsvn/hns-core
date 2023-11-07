/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_TRANSACTION_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_TRANSACTION_H_

#include <string>

#include "base/types/expected.h"
#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

enum class GetSKUTransactionError { kDatabaseError, kTransactionNotFound };

using GetSKUTransactionCallback = std::function<void(
    base::expected<mojom::SKUTransactionPtr, GetSKUTransactionError>)>;

class DatabaseSKUTransaction : public DatabaseTable {
 public:
  explicit DatabaseSKUTransaction(RewardsEngineImpl& engine);
  ~DatabaseSKUTransaction() override;

  void InsertOrUpdate(mojom::SKUTransactionPtr info,
                      LegacyResultCallback callback);

  void SaveExternalTransaction(const std::string& transaction_id,
                               const std::string& external_transaction_id,
                               LegacyResultCallback callback);

  void GetRecordByOrderId(const std::string& order_id,
                          GetSKUTransactionCallback callback);

 private:
  void OnGetRecord(GetSKUTransactionCallback callback,
                   mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_TRANSACTION_H_
