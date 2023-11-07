/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/gemini/gemini_server.h"

#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {
namespace endpoint {

GeminiServer::GeminiServer(RewardsEngineImpl& engine)
    : post_account_(engine),
      post_balance_(engine),
      post_oauth_(engine),
      post_recipient_id_(engine) {}

GeminiServer::~GeminiServer() = default;

}  // namespace endpoint
}  // namespace hns_rewards::internal
