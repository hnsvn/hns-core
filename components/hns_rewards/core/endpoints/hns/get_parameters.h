/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_GET_PARAMETERS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_GET_PARAMETERS_H_

#include <string>

#include "hns/components/hns_rewards/common/mojom/rewards.mojom.h"
#include "hns/components/hns_rewards/common/mojom/rewards_endpoints.mojom.h"
#include "hns/components/hns_rewards/core/endpoints/request_builder.h"
#include "hns/components/hns_rewards/core/endpoints/response_handler.h"
#include "hns/components/hns_rewards/core/endpoints/result_for.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// GET /v1/parameters
//
// Request body: -
//
// clang-format off
// Response body:
// {
//   "autocontribute": {
//     "choices": [1, 2, 3, 5, 7, 10, 20],
//     "defaultChoice": 1
//   },
//   "batRate": 0.301298,
//   "custodianRegions": {
//     "bitflyer": {
//       "allow": ["JP"],
//       "block": []
//     },
//     "gemini": {
//       "allow": ["AU", "AT", "BE", "CA", "CO", "DK", "FI", "HK", "IE", "IT", "NL", "NO", "PT", "SG", "ES", "SE", "GB", "US"],  // NOLINT
//       "block": []
//     },
//     "uphold": {
//       "allow": ["AU", "AT", "BE", "CO", "DK", "FI", "HK", "IE", "IT", "NL", "NO", "PT", "SG", "ES", "SE", "GB", "US"],  // NOLINT
//       "block": []
//     }
//   },
//   "payoutStatus": {
//     "bitflyer": "complete",
//     "gemini": "complete",
//     "unverified": "complete",
//     "uphold": "complete"
//   },
//   "tips": {
//     "defaultMonthlyChoices": [1.25, 5, 10.5],
//     "defaultTipChoices": [1.25, 5, 10.5]
//   },
//   "vbatDeadline": "2022-12-24T15:04:45.352584Z",
//   "vbatExpired": false
// }
// clang-format on

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class GetParameters;

template <>
struct ResultFor<GetParameters> {
  using Value = mojom::RewardsParametersPtr;
  using Error = mojom::GetParametersError;
};

class GetParameters final : public RequestBuilder,
                            public ResponseHandler<GetParameters> {
 public:
  static Result ProcessResponse(const mojom::UrlResponse&);

  explicit GetParameters(RewardsEngineImpl& engine);
  ~GetParameters() override;

 private:
  absl::optional<std::string> Url() const override;
  mojom::UrlMethod Method() const override;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_GET_PARAMETERS_H_
