/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_RESPONSE_HANDLER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_RESPONSE_HANDLER_H_

#include <utility>

#include "base/functional/callback.h"
#include "base/types/expected.h"
#include "hns/components/hns_rewards/core/endpoints/result_for.h"
#include "hns/components/hns_rewards/core/logging/logging_util.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal::endpoints {

template <typename Endpoint>
class ResponseHandler {
 public:
  using Value = typename ResultFor<Endpoint>::Value;
  using Error = typename ResultFor<Endpoint>::Error;
  using Result = base::expected<Value, Error>;

 private:
  static void OnResponse(base::OnceCallback<void(Result&&)> callback,
                         mojom::UrlResponsePtr response) {
    DCHECK(response);
    LogUrlResponse(__func__, *response);
    std::move(callback).Run(Endpoint::ProcessResponse(*response));
  }

  // Note that friend class RequestFor<Endpoint>; is not sufficient due to
  // class hierarchies implementing an endpoint (e.g. PostConnect is the one
  // that derives from ResponseHandler<PostConnect>, but we're passing
  // PostConnectBitflyer, PostConnectGemini and PostConnectUphold to
  // RequestFor<>).
  template <typename>
  friend class RequestFor;
};

}  // namespace hns_rewards::internal::endpoints

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_RESPONSE_HANDLER_H_
