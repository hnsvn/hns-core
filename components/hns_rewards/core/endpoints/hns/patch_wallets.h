/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_PATCH_WALLETS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_PATCH_WALLETS_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/common/mojom/rewards_endpoints.mojom.h"
#include "hns/components/hns_rewards/core/endpoints/request_builder.h"
#include "hns/components/hns_rewards/core/endpoints/response_handler.h"
#include "hns/components/hns_rewards/core/endpoints/result_for.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// PATCH /v4/wallets/<rewards_payment_id>
//
// Request body:
// {
//   "geo_country": "US"
// }
//
// Response body: -

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace endpoints {

class PatchWallets;

template <>
struct ResultFor<PatchWallets> {
  using Value = void;
  using Error = mojom::PatchWalletsError;
};

class PatchWallets final : public RequestBuilder,
                           public ResponseHandler<PatchWallets> {
 public:
  static Result ProcessResponse(const mojom::UrlResponse&);

  PatchWallets(RewardsEngineImpl& engine, std::string&& geo_country);
  ~PatchWallets() override;

 private:
  const char* Path() const;

  absl::optional<std::string> Url() const override;
  mojom::UrlMethod Method() const override;
  absl::optional<std::vector<std::string>> Headers(
      const std::string& content) const override;
  absl::optional<std::string> Content() const override;
  std::string ContentType() const override;

  std::string geo_country_;
};

}  // namespace endpoints
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_PATCH_WALLETS_H_
