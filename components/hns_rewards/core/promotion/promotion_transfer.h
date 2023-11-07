/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_PROMOTION_PROMOTION_TRANSFER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_PROMOTION_PROMOTION_TRANSFER_H_

#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/credentials/credentials_promotion.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace promotion {

class PromotionTransfer {
 public:
  explicit PromotionTransfer(RewardsEngineImpl& engine);
  ~PromotionTransfer();

  void Start(PostSuggestionsClaimCallback callback);

 private:
  void OnGetSpendableUnblindedTokens(
      PostSuggestionsClaimCallback callback,
      std::vector<mojom::UnblindedTokenPtr> tokens);

  void OnDrainTokens(PostSuggestionsClaimCallback callback,
                     double transfer_amount,
                     mojom::Result result,
                     std::string drain_id) const;

  const raw_ref<RewardsEngineImpl> engine_;
  credential::CredentialsPromotion credentials_;
};

}  // namespace promotion
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_PROMOTION_PROMOTION_TRANSFER_H_
