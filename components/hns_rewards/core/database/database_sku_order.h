/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_ORDER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_ORDER_H_

#include <memory>
#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_sku_order_items.h"
#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

using GetSKUOrderCallback = std::function<void(mojom::SKUOrderPtr)>;

class DatabaseSKUOrder : public DatabaseTable {
 public:
  explicit DatabaseSKUOrder(RewardsEngineImpl& engine);
  ~DatabaseSKUOrder() override;

  void InsertOrUpdate(mojom::SKUOrderPtr info, LegacyResultCallback callback);

  void UpdateStatus(const std::string& order_id,
                    mojom::SKUOrderStatus status,
                    LegacyResultCallback callback);

  void GetRecord(const std::string& order_id, GetSKUOrderCallback callback);

  void GetRecordByContributionId(const std::string& contribution_id,
                                 GetSKUOrderCallback callback);

  void SaveContributionIdForSKUOrder(const std::string& order_id,
                                     const std::string& contribution_id,
                                     LegacyResultCallback callback);

 private:
  void OnGetRecord(GetSKUOrderCallback callback,
                   mojom::DBCommandResponsePtr response);

  void OnGetRecordItems(std::vector<mojom::SKUOrderItemPtr> list,
                        std::shared_ptr<mojom::SKUOrderPtr> shared_order,
                        GetSKUOrderCallback callback);

  DatabaseSKUOrderItems items_;
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_SKU_ORDER_H_
