/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_ZEBPAY_GET_BALANCE_ZEBPAY_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_ZEBPAY_GET_BALANCE_ZEBPAY_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/common/mojom/rewards_endpoints.mojom.h"
#include "hns/components/hns_rewards/core/endpoints/request_builder.h"
#include "hns/components/hns_rewards/core/endpoints/response_handler.h"
#include "hns/components/hns_rewards/core/endpoints/result_for.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// GET /api/balance
//
// Request body:
// -
//
// Response body:
// {
//   "BAT": 0
// }

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class GetBalanceZebPay;

template <>
struct ResultFor<GetBalanceZebPay> {
  using Value = double;  // balance
  using Error = mojom::GetBalanceZebPayError;
};

class GetBalanceZebPay final : public RequestBuilder,
                               public ResponseHandler<GetBalanceZebPay> {
 public:
  static Result ProcessResponse(const mojom::UrlResponse& response);

  GetBalanceZebPay(RewardsEngineImpl& engine, std::string&& token);
  ~GetBalanceZebPay() override;

 private:
  absl::optional<std::string> Url() const override;
  mojom::UrlMethod Method() const override;
  absl::optional<std::vector<std::string>> Headers(
      const std::string& content) const override;

  std::string token_;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_ZEBPAY_GET_BALANCE_ZEBPAY_H_
