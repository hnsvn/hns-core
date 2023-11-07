/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_PUT_SAFETYNET_PUT_SAFETYNET_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_PUT_SAFETYNET_PUT_SAFETYNET_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// PUT /v2/attestations/safetynet/{nonce}
//
// Request body:
// {
//   "token": "dfasdfasdpflsadfplf2r23re2"
// }
//
// Success:
// HTTP_OK (200)
//
// Error codes:
// HTTP_BAD_REQUEST (400)
// HTTP_UNAUTHORIZED (401)
// HTTP_INTERNAL_SERVER_ERROR (500)
//
// Response body (success):
// {Empty}
//
// Response body (error):
// {
//   "message": "Error solving captcha",
//   "code": 401
// }

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace promotion {

using PutSafetynetCallback = base::OnceCallback<void(mojom::Result)>;

class PutSafetynet {
 public:
  explicit PutSafetynet(RewardsEngineImpl& engine);
  ~PutSafetynet();

  void Request(const std::string& token,
               const std::string& nonce,
               PutSafetynetCallback callback);

 private:
  std::string GetUrl(const std::string& nonce);

  std::string GeneratePayload(const std::string& token);

  mojom::Result CheckStatusCode(const int status_code);

  void OnRequest(PutSafetynetCallback callback, mojom::UrlResponsePtr response);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_PUT_SAFETYNET_PUT_SAFETYNET_H_
