/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PRIVATE_CDN_PRIVATE_CDN_SERVER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PRIVATE_CDN_PRIVATE_CDN_SERVER_H_

#include "hns/components/hns_rewards/core/endpoint/private_cdn/get_publisher/get_publisher.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {

class PrivateCDNServer {
 public:
  explicit PrivateCDNServer(RewardsEngineImpl& engine);
  ~PrivateCDNServer();

  private_cdn::GetPublisher& get_publisher() { return get_publisher_; }

 private:
  private_cdn::GetPublisher get_publisher_;
};

}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PRIVATE_CDN_PRIVATE_CDN_SERVER_H_
