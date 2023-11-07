/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PRIVATE_CDN_GET_PUBLISHER_GET_PUBLISHER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PRIVATE_CDN_GET_PUBLISHER_GET_PUBLISHER_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// GET /publishers/prefixes/{prefix}
//
// Success code:
// HTTP_OK (200)
//
// Error codes:
// HTTP_NOT_FOUND (404)
//
// Response body:
// See
// https://github.com/hnsvn/hns-core/blob/master/components/hns_rewards/core/publisher/protos/channel_response.proto

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace private_cdn {

using GetPublisherCallback =
    std::function<void(const mojom::Result result,
                       mojom::ServerPublisherInfoPtr info)>;

class GetPublisher {
 public:
  explicit GetPublisher(RewardsEngineImpl& engine);
  ~GetPublisher();

  void Request(const std::string& publisher_key,
               const std::string& hash_prefix,
               GetPublisherCallback callback);

 private:
  std::string GetUrl(const std::string& hash_prefix);

  mojom::Result CheckStatusCode(const int status_code);

  mojom::Result ParseBody(const std::string& body,
                          const std::string& publisher_key,
                          mojom::ServerPublisherInfo* info);

  void OnRequest(mojom::UrlResponsePtr response,
                 const std::string& publisher_key,
                 GetPublisherCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace private_cdn
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PRIVATE_CDN_GET_PUBLISHER_GET_PUBLISHER_H_
