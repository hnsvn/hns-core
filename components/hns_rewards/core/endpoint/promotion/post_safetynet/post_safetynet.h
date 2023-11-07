/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_SAFETYNET_POST_SAFETYNET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_SAFETYNET_POST_SAFETYNET_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// POST /v2/attestations/safetynet
//
// Request body:
// {
//   "paymentIds": [
//     "83b3b77b-e7c3-455b-adda-e476fa0656d2"
//   ]
// }
//
// Success code:
// HTTP_OK (200)
//
// Error codes:
// HTTP_BAD_REQUEST (400)
// HTTP_UNAUTHORIZED (401)
//
// Response body:
// {
//   "nonce": "c4645786-052f-402f-8593-56af2f7a21ce"
// }

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace promotion {

using PostSafetynetCallback =
    base::OnceCallback<void(mojom::Result result, const std::string& nonce)>;

class PostSafetynet {
 public:
  explicit PostSafetynet(RewardsEngineImpl& engine);
  ~PostSafetynet();

  void Request(PostSafetynetCallback callback);

 private:
  std::string GetUrl();

  std::string GeneratePayload();

  mojom::Result CheckStatusCode(const int status_code);

  mojom::Result ParseBody(const std::string& body, std::string* nonce);

  void OnRequest(PostSafetynetCallback callback,
                 mojom::UrlResponsePtr response);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_SAFETYNET_POST_SAFETYNET_H_
