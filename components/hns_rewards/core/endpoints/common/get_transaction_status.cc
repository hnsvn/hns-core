/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoints/common/get_transaction_status.h"

#include <utility>

namespace hns_rewards::internal::endpoints {

GetTransactionStatus::GetTransactionStatus(RewardsEngineImpl& engine,
                                           std::string&& token,
                                           std::string&& transaction_id)
    : RequestBuilder(engine),
      token_(std::move(token)),
      transaction_id_(std::move(transaction_id)) {}

GetTransactionStatus::~GetTransactionStatus() = default;

mojom::UrlMethod GetTransactionStatus::Method() const {
  return mojom::UrlMethod::GET;
}

}  // namespace hns_rewards::internal::endpoints
