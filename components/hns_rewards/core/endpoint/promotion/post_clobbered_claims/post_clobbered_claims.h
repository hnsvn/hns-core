/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_CLOBBERED_CLAIMS_POST_CLOBBERED_CLAIMS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_CLOBBERED_CLAIMS_POST_CLOBBERED_CLAIMS_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "base/values.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// POST /v1/promotions/reportclobberedclaims
//
// Request body:
// {
//   "claimIds": ["asfeq4gerg34gl3g34lg34g"]
// }
//
// Success code:
// HTTP_OK (200)
//
// Error Codes:
// HTTP_BAD_REQUEST (400)
// HTTP_INTERNAL_SERVER_ERROR (500)
//
// Response body:
// {Empty}

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace promotion {

using PostClobberedClaimsCallback =
    std::function<void(const mojom::Result result)>;

class PostClobberedClaims {
 public:
  explicit PostClobberedClaims(RewardsEngineImpl& engine);
  ~PostClobberedClaims();

  void Request(base::Value::List corrupted_claims,
               PostClobberedClaimsCallback callback);

 private:
  std::string GetUrl();

  std::string GeneratePayload(base::Value::List corrupted_claims);

  mojom::Result CheckStatusCode(const int status_code);

  void OnRequest(mojom::UrlResponsePtr response,
                 PostClobberedClaimsCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_CLOBBERED_CLAIMS_POST_CLOBBERED_CLAIMS_H_
