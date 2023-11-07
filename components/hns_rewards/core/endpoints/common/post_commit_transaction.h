/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_COMMON_POST_COMMIT_TRANSACTION_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_COMMON_POST_COMMIT_TRANSACTION_H_

#include <string>

#include "hns/components/hns_rewards/core/endpoints/request_builder.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class PostCommitTransaction : public RequestBuilder {
 public:
  PostCommitTransaction(RewardsEngineImpl& engine,
                        std::string&& token,
                        std::string&& address,
                        mojom::ExternalTransactionPtr);
  ~PostCommitTransaction() override;

 protected:
  std::string token_;
  std::string address_;
  mojom::ExternalTransactionPtr transaction_;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_COMMON_POST_COMMIT_TRANSACTION_H_
