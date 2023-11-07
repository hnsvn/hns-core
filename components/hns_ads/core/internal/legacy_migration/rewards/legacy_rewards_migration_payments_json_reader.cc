/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/rewards/legacy_rewards_migration_payments_json_reader.h"

#include "base/json/json_reader.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/legacy_migration/rewards/legacy_rewards_migration_payments_json_reader_util.h"

namespace hns_ads::rewards::json::reader {

absl::optional<PaymentList> ReadPayments(const std::string& json) {
  const absl::optional<base::Value> root = base::JSONReader::Read(json);
  if (!root || !root->is_dict()) {
    return absl::nullopt;
  }

  return ParsePayments(root->GetDict());
}

}  // namespace hns_ads::rewards::json::reader
