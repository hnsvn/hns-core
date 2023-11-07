/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_PROMOTION_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_PROMOTION_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

using GetPromotionCallback = std::function<void(mojom::PromotionPtr)>;

using GetPromotionListCallback =
    std::function<void(std::vector<mojom::PromotionPtr>)>;

class DatabasePromotion : public DatabaseTable {
 public:
  explicit DatabasePromotion(RewardsEngineImpl& engine);
  ~DatabasePromotion() override;

  void InsertOrUpdate(mojom::PromotionPtr info, LegacyResultCallback callback);

  void GetRecord(const std::string& id, GetPromotionCallback callback);

  void GetRecords(const std::vector<std::string>& ids,
                  GetPromotionListCallback callback);

  void GetAllRecords(GetAllPromotionsCallback callback);

  void SaveClaimId(const std::string& promotion_id,
                   const std::string& claim_id,
                   LegacyResultCallback callback);

  void UpdateStatus(const std::string& promotion_id,
                    mojom::PromotionStatus status,
                    LegacyResultCallback callback);

  void UpdateRecordsStatus(const std::vector<std::string>& ids,
                           mojom::PromotionStatus status,
                           LegacyResultCallback callback);

  void CredentialCompleted(const std::string& promotion_id,
                           LegacyResultCallback callback);

  void UpdateRecordsBlankPublicKey(const std::vector<std::string>& ids,
                                   LegacyResultCallback callback);

 private:
  void OnGetRecord(GetPromotionCallback callback,
                   mojom::DBCommandResponsePtr response);

  void OnGetAllRecords(GetAllPromotionsCallback callback,
                       mojom::DBCommandResponsePtr response);

  void OnGetRecords(GetPromotionListCallback callback,
                    mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_PROMOTION_H_
