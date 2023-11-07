/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_SUGGESTIONS_POST_SUGGESTIONS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_SUGGESTIONS_POST_SUGGESTIONS_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/credentials/credentials_redeem.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

// POST /v1/suggestions
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
//   "suggestion": "base64_string"
// }
//
// Success code:
// HTTP_OK (200)
//
// Error codes:
// HTTP_BAD_REQUEST (400)
// HTTP_SERVICE_UNAVAILABLE (503)
//
// Response body:
// {Empty}

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {
namespace promotion {

using PostSuggestionsCallback = std::function<void(const mojom::Result result)>;

class PostSuggestions {
 public:
  explicit PostSuggestions(RewardsEngineImpl& engine);
  ~PostSuggestions();

  void Request(const credential::CredentialsRedeem& redeem,
               PostSuggestionsCallback callback);

 private:
  std::string GetUrl();

  std::string GeneratePayload(const credential::CredentialsRedeem& redeem);

  mojom::Result CheckStatusCode(const int status_code);

  void OnRequest(mojom::UrlResponsePtr response,
                 PostSuggestionsCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_PROMOTION_POST_SUGGESTIONS_POST_SUGGESTIONS_H_
