/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_UPHOLD_UPHOLD_SERVER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_UPHOLD_UPHOLD_SERVER_H_

#include "hns/components/hns_rewards/core/endpoint/uphold/get_capabilities/get_capabilities.h"
#include "hns/components/hns_rewards/core/endpoint/uphold/get_card/get_card.h"
#include "hns/components/hns_rewards/core/endpoint/uphold/get_cards/get_cards.h"
#include "hns/components/hns_rewards/core/endpoint/uphold/get_me/get_me.h"
#include "hns/components/hns_rewards/core/endpoint/uphold/patch_card/patch_card.h"
#include "hns/components/hns_rewards/core/endpoint/uphold/post_cards/post_cards.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {

class UpholdServer {
 public:
  explicit UpholdServer(RewardsEngineImpl& engine);
  ~UpholdServer();

  const uphold::GetCapabilities& get_capabilities() const {
    return get_capabilities_;
  }

  const uphold::GetCards& get_cards() const { return get_cards_; }

  uphold::GetCard& get_card() { return get_card_; }

  uphold::GetMe& get_me() { return get_me_; }

  const uphold::PostCards& post_cards() const { return post_cards_; }

  const uphold::PatchCard& patch_card() const { return patch_card_; }

 private:
  uphold::GetCapabilities get_capabilities_;
  uphold::GetCards get_cards_;
  uphold::GetCard get_card_;
  uphold::GetMe get_me_;
  uphold::PostCards post_cards_;
  uphold::PatchCard patch_card_;
};

}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_UPHOLD_UPHOLD_SERVER_H_
