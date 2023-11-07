/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MULTI_TABLES_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MULTI_TABLES_H_

#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace database {

class DatabaseMultiTables {
 public:
  explicit DatabaseMultiTables(RewardsEngineImpl& engine);
  ~DatabaseMultiTables();

  void GetTransactionReport(const mojom::ActivityMonth month,
                            const int year,
                            GetTransactionReportCallback callback);

 private:
  void OnGetTransactionReportPromotion(
      base::flat_map<std::string, mojom::PromotionPtr> promotions,
      const mojom::ActivityMonth month,
      const int year,
      GetTransactionReportCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MULTI_TABLES_H_
