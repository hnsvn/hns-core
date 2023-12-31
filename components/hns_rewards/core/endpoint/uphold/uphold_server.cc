/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/uphold/uphold_server.h"

#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {
namespace endpoint {

UpholdServer::UpholdServer(RewardsEngineImpl& engine)
    : get_capabilities_(engine),
      get_cards_(engine),
      get_card_(engine),
      get_me_(engine),
      post_cards_(engine),
      patch_card_(engine) {}

UpholdServer::~UpholdServer() = default;

}  // namespace endpoint
}  // namespace hns_rewards::internal
