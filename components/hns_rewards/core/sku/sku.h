/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_H_

#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "hns/components/hns_rewards/core/sku/sku_common.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace sku {

class SKU {
 public:
  explicit SKU(RewardsEngineImpl& engine);
  ~SKU();

  void Process(const std::vector<mojom::SKUOrderItem>& items,
               const std::string& wallet_type,
               SKUOrderCallback callback,
               const std::string& contribution_id = "");

  void Retry(const std::string& order_id,
             const std::string& wallet_type,
             SKUOrderCallback callback);

 private:
  void OrderCreated(const mojom::Result result,
                    const std::string& order_id,
                    const std::string& wallet_type,
                    const std::string& contribution_id,
                    SKUOrderCallback callback);

  void ContributionIdSaved(const mojom::Result result,
                           const std::string& order_id,
                           const std::string& wallet_type,
                           SKUOrderCallback callback);

  void CreateTransaction(mojom::SKUOrderPtr order,
                         const std::string& wallet_type,
                         SKUOrderCallback callback);

  void OnOrder(mojom::SKUOrderPtr order,
               const std::string& wallet_type,
               SKUOrderCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
  SKUCommon common_;
};

}  // namespace sku
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_H_
