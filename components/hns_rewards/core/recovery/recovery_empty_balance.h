/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_RECOVERY_RECOVERY_EMPTY_BALANCE_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_RECOVERY_RECOVERY_EMPTY_BALANCE_H_

#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/database/database_promotion.h"
#include "hns/components/hns_rewards/core/endpoint/promotion/promotion_server.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace recovery {

class EmptyBalance {
 public:
  explicit EmptyBalance(RewardsEngineImpl& engine);
  ~EmptyBalance();

  void Check();

 private:
  void OnAllContributions(std::vector<mojom::ContributionInfoPtr> list);

  void GetPromotions(database::GetPromotionListCallback callback);

  void OnPromotions(base::flat_map<std::string, mojom::PromotionPtr> promotions,
                    database::GetPromotionListCallback callback);

  void GetCredsByPromotions(std::vector<mojom::PromotionPtr> list);

  void OnCreds(std::vector<mojom::CredsBatchPtr> list);

  void OnSaveUnblindedCreds(const mojom::Result result);

  void GetAllTokens(std::vector<mojom::PromotionPtr> list,
                    const double contribution_sum);

  void ReportResults(std::vector<mojom::UnblindedTokenPtr> list,
                     const double contribution_sum,
                     const double promotion_sum);

  void Sent(const mojom::Result result);

  const raw_ref<RewardsEngineImpl> engine_;
  endpoint::PromotionServer promotion_server_;
};

}  // namespace recovery
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_RECOVERY_RECOVERY_EMPTY_BALANCE_H_
