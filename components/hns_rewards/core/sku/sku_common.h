/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_COMMON_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_COMMON_H_

#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "base/types/expected.h"
#include "hns/components/hns_rewards/core/database/database_sku_transaction.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "hns/components/hns_rewards/core/sku/sku_order.h"
#include "hns/components/hns_rewards/core/sku/sku_transaction.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace sku {

class SKUCommon {
 public:
  explicit SKUCommon(RewardsEngineImpl& engine);
  ~SKUCommon();

  void CreateOrder(const std::vector<mojom::SKUOrderItem>& items,
                   SKUOrderCallback callback);

  void CreateTransaction(mojom::SKUOrderPtr order,
                         const std::string& destination,
                         const std::string& wallet_type,
                         SKUOrderCallback callback);

  void SendExternalTransaction(const std::string& order_id,
                               SKUOrderCallback callback);

 private:
  void OnTransactionCompleted(const mojom::Result result,
                              const std::string& order_id,
                              SKUOrderCallback callback);

  void GetSKUTransactionByOrderId(
      base::expected<mojom::SKUTransactionPtr, database::GetSKUTransactionError>
          result,
      SKUOrderCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
  SKUOrder order_;
  SKUTransaction transaction_;
};

}  // namespace sku
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_COMMON_H_
