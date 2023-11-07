/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_PAYMENT_TOKENS_JSON_READER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_PAYMENT_TOKENS_JSON_READER_H_

#include <string>

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::rewards::json::reader {

absl::optional<PaymentTokenList> ReadPaymentTokens(const std::string& json);

}  // namespace hns_ads::rewards::json::reader

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_PAYMENT_TOKENS_JSON_READER_H_
