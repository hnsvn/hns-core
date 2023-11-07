/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/summary_user_data.h"

#include <utility>

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/summary_user_data_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {

constexpr char kSummaryKey[] = "totals";
constexpr char kAdFormatKey[] = "ad_format";

}  // namespace

base::Value::Dict BuildSummaryUserData(const PaymentTokenList& payment_tokens) {
  base::Value::Dict user_data;

  if (!UserHasJoinedHnsRewards()) {
    return user_data;
  }

  const AdTypeBucketMap buckets = BuildBuckets(payment_tokens);

  base::Value::List list;

  for (const auto& [ad_format, confirmations] : buckets) {
    auto dict = base::Value::Dict().Set(kAdFormatKey, ad_format);

    for (const auto& [confirmation_type, count] : confirmations) {
      dict.Set(confirmation_type, count);
    }

    list.Append(std::move(dict));
  }

  user_data.Set(kSummaryKey, std::move(list));

  return user_data;
}

}  // namespace hns_ads
