/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_REWARDS_GET_PREFIX_LIST_GET_PREFIX_LIST_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_REWARDS_GET_PREFIX_LIST_GET_PREFIX_LIST_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// GET /publishers/prefix-list
//
// Success code:
// HTTP_OK (200)
//
// Response body:
// blob

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace rewards {

using GetPrefixListCallback =
    std::function<void(const mojom::Result result, const std::string& body)>;

class GetPrefixList {
 public:
  explicit GetPrefixList(RewardsEngineImpl& engine);
  ~GetPrefixList();

  void Request(GetPrefixListCallback callback);

 private:
  std::string GetUrl();

  mojom::Result CheckStatusCode(const int status_code);

  void OnRequest(mojom::UrlResponsePtr response,
                 GetPrefixListCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace rewards
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_REWARDS_GET_PREFIX_LIST_GET_PREFIX_LIST_H_
