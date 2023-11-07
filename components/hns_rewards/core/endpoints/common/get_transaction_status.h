/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_COMMON_GET_TRANSACTION_STATUS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_COMMON_GET_TRANSACTION_STATUS_H_

#include <string>

#include "hns/components/hns_rewards/core/endpoints/request_builder.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class GetTransactionStatus : public RequestBuilder {
 public:
  GetTransactionStatus(RewardsEngineImpl& engine,
                       std::string&& token,
                       std::string&& transaction_id);
  ~GetTransactionStatus() override;

 private:
  mojom::UrlMethod Method() const override;

 protected:
  std::string token_;
  std::string transaction_id_;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_COMMON_GET_TRANSACTION_STATUS_H_
