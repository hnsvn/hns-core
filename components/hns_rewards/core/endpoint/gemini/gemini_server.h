/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_GEMINI_GEMINI_SERVER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_GEMINI_GEMINI_SERVER_H_

#include "hns/components/hns_rewards/core/endpoint/gemini/post_account/post_account_gemini.h"
#include "hns/components/hns_rewards/core/endpoint/gemini/post_balance/post_balance_gemini.h"
#include "hns/components/hns_rewards/core/endpoint/gemini/post_oauth/post_oauth_gemini.h"
#include "hns/components/hns_rewards/core/endpoint/gemini/post_recipient_id/post_recipient_id_gemini.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoint {

class GeminiServer {
 public:
  explicit GeminiServer(RewardsEngineImpl& engine);
  ~GeminiServer();

  gemini::PostAccount& post_account() { return post_account_; }

  gemini::PostBalance& post_balance() { return post_balance_; }

  gemini::PostOauth& post_oauth() { return post_oauth_; }

  gemini::PostRecipientId& post_recipient_id() { return post_recipient_id_; }

 private:
  gemini::PostAccount post_account_;
  gemini::PostBalance post_balance_;
  gemini::PostOauth post_oauth_;
  gemini::PostRecipientId post_recipient_id_;
};

}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_GEMINI_GEMINI_SERVER_H_
