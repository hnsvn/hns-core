/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoints/hns/post_connect_zebpay.h"

#include <utility>

#include "base/json/json_writer.h"
#include "hns/components/hns_rewards/core/logging/logging.h"

namespace hns_rewards::internal::endpoints {

PostConnectZebPay::PostConnectZebPay(RewardsEngineImpl& engine,
                                     std::string&& linking_info)
    : PostConnect(engine), linking_info_(std::move(linking_info)) {}

PostConnectZebPay::~PostConnectZebPay() = default;

absl::optional<std::string> PostConnectZebPay::Content() const {
  if (linking_info_.empty()) {
    BLOG(0, "linking_info_ is empty!");
    return absl::nullopt;
  }

  base::Value::Dict content;
  content.Set("linking_info", linking_info_);

  std::string json;
  if (!base::JSONWriter::Write(content, &json)) {
    BLOG(0, "Failed to write content to JSON!");
    return absl::nullopt;
  }

  return json;
}

const char* PostConnectZebPay::Path() const {
  return "/v3/wallet/zebpay/%s/claim";
}

}  // namespace hns_rewards::internal::endpoints
