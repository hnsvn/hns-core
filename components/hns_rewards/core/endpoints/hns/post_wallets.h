/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_POST_WALLETS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_POST_WALLETS_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/common/mojom/rewards_endpoints.mojom.h"
#include "hns/components/hns_rewards/core/endpoints/request_builder.h"
#include "hns/components/hns_rewards/core/endpoints/response_handler.h"
#include "hns/components/hns_rewards/core/endpoints/result_for.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// POST /v4/wallets
//
// Request body:
// {
//   "geo_country": "US"
// }
//
// clang-format off
// Response body:
// {
//   "paymentId": "33fe956b-ed15-515b-bccd-b6cc63a80e0e"
// }
// clang-format on

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class PostWallets;

template <>
struct ResultFor<PostWallets> {
  using Value = std::string;  // Rewards payment ID
  using Error = mojom::PostWalletsError;
};

class PostWallets final : public RequestBuilder,
                          public ResponseHandler<PostWallets> {
 public:
  static Result ProcessResponse(const mojom::UrlResponse&);

  PostWallets(RewardsEngineImpl& engine,
              absl::optional<std::string>&& geo_country);
  ~PostWallets() override;

 private:
  const char* Path() const;

  absl::optional<std::string> Url() const override;
  absl::optional<std::vector<std::string>> Headers(
      const std::string& content) const override;
  absl::optional<std::string> Content() const override;
  std::string ContentType() const override;

  absl::optional<std::string> geo_country_;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_POST_WALLETS_H_
