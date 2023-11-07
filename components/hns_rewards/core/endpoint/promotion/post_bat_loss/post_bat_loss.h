/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_BAT_LOSS_POST_BAT_LOSS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_BAT_LOSS_POST_BAT_LOSS_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// POST /v1/wallets/{payment_id}/events/batloss/{version}
//
// Request body:
// {
//   "amount": 20.5
// }
//
// Success code:
// HTTP_OK (200)
//
// Error Codes:
// HTTP_INTERNAL_SERVER_ERROR (500)
//
// Response body:
// {Empty}

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace promotion {

using PostBatLossCallback = std::function<void(const mojom::Result result)>;

class PostBatLoss {
 public:
  explicit PostBatLoss(RewardsEngineImpl& engine);
  ~PostBatLoss();

  void Request(const double amount,
               const int32_t version,
               PostBatLossCallback callback);

 private:
  std::string GetUrl(const int32_t version);

  std::string GeneratePayload(const double amount);

  mojom::Result CheckStatusCode(const int status_code);

  void OnRequest(mojom::UrlResponsePtr response, PostBatLossCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_BAT_LOSS_POST_BAT_LOSS_H_
