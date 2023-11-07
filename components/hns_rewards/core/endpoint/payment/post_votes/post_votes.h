/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PAYMENT_POST_VOTES_POST_VOTES_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PAYMENT_POST_VOTES_POST_VOTES_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/credentials/credentials_redeem.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// POST /v1/votes
//
// Request body:
// {
//   "credentials": [
//     {
//       "t": "",
//       "publicKey": "",
//       "signature": ""
//     }
//   ],
//   "vote": "base64_string"
// }
//
// Success code:
// HTTP_CREATED (200)
//
// Error codes:
// HTTP_BAD_REQUEST (400)
// HTTP_INTERNAL_SERVER_ERROR (500)
//
// Response body:
// {Empty}

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace payment {

using PostVotesCallback = std::function<void(const mojom::Result result)>;

class PostVotes {
 public:
  explicit PostVotes(RewardsEngineImpl& engine);
  ~PostVotes();

  void Request(const credential::CredentialsRedeem& redeem,
               PostVotesCallback callback);

 private:
  std::string GetUrl();

  std::string GeneratePayload(const credential::CredentialsRedeem& redeem);

  mojom::Result CheckStatusCode(const int status_code);

  void OnRequest(mojom::UrlResponsePtr response, PostVotesCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace payment
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PAYMENT_POST_VOTES_POST_VOTES_H_
