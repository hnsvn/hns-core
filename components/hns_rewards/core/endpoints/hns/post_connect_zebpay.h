/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_POST_CONNECT_ZEBPAY_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_POST_CONNECT_ZEBPAY_H_

#include <string>

#include "hns/components/hns_rewards/core/endpoints/common/post_connect.h"

// POST /v3/wallet/zebpay/{rewards_payment_id}/claim
//
// clang-format off
// Request body:
// {
//   "linkingInfo": "..."
// }
// clang-format on
//
// Response body: -

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class PostConnectZebPay final : public PostConnect {
 public:
  PostConnectZebPay(RewardsEngineImpl& engine, std::string&& linking_info);
  ~PostConnectZebPay() override;

 private:
  absl::optional<std::string> Content() const override;

  const char* Path() const override;

  std::string linking_info_;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_POST_CONNECT_ZEBPAY_H_
