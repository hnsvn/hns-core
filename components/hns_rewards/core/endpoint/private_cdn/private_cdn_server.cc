/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "hns/components/hns_rewards/core/endpoint/private_cdn/private_cdn_server.h"

#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {
namespace endpoint {

PrivateCDNServer::PrivateCDNServer(RewardsEngineImpl& engine)
    : get_publisher_(engine) {}

PrivateCDNServer::~PrivateCDNServer() = default;

}  // namespace endpoint
}  // namespace hns_rewards::internal
