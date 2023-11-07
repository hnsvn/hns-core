/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MOCK_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MOCK_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace hns_rewards::internal {
namespace database {

class MockDatabase : public Database {
 public:
  explicit MockDatabase(RewardsEngineImpl& engine);

  ~MockDatabase() override;

  MOCK_METHOD2(GetContributionInfo,
               void(const std::string& contribution_id,
                    GetContributionInfoCallback callback));

  MOCK_METHOD2(GetSpendableUnblindedTokensByBatchTypes,
               void(const std::vector<mojom::CredsBatchType>& batch_types,
                    GetUnblindedTokenListCallback callback));

  MOCK_METHOD2(SavePromotion,
               void(mojom::PromotionPtr info, LegacyResultCallback callback));

  MOCK_METHOD1(GetAllPromotions, void(GetAllPromotionsCallback callback));
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_MOCK_H_
