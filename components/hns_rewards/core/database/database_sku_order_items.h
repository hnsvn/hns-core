/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_ORDER_ITEMS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_ORDER_ITEMS_H_

#include <memory>
#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

using GetSKUOrderItemsCallback =
    std::function<void(std::vector<mojom::SKUOrderItemPtr>)>;

class DatabaseSKUOrderItems : public DatabaseTable {
 public:
  explicit DatabaseSKUOrderItems(RewardsEngineImpl& engine);
  ~DatabaseSKUOrderItems() override;

  void InsertOrUpdateList(mojom::DBTransaction* transaction,
                          std::vector<mojom::SKUOrderItemPtr> list);

  void GetRecordsByOrderId(const std::string& order_id,
                           GetSKUOrderItemsCallback callback);

 private:
  void OnGetRecordsByOrderId(GetSKUOrderItemsCallback callback,
                             mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_ORDER_ITEMS_H_
