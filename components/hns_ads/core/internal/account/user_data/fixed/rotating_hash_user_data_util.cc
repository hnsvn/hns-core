/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/rotating_hash_user_data_util.h"

#include <cinttypes>
#include <cstdint>

#include "base/base64.h"
#include "base/strings/stringprintf.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/common/crypto/crypto_util.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"

namespace hns_ads {

absl::optional<std::string> BuildRotatingHash(
    const TransactionInfo& transaction) {
  const std::string& device_id =
      GlobalState::GetInstance()->SysInfo().device_id;
  if (device_id.empty()) {
    return absl::nullopt;
  }

  const int64_t timestamp_rounded_down_to_nearest_hour =
      base::Time::Now().ToDeltaSinceWindowsEpoch().InSeconds() /
      base::Time::kSecondsPerHour;

  return base::Base64Encode(crypto::Sha256(
      base::StringPrintf("%s%s%" PRId64, device_id.c_str(),
                         transaction.creative_instance_id.c_str(),
                         timestamp_rounded_down_to_nearest_hour)));
}

}  // namespace hns_ads
