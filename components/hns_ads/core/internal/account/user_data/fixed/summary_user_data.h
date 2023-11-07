/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_SUMMARY_USER_DATA_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_SUMMARY_USER_DATA_H_

#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"

namespace hns_ads {

base::Value::Dict BuildSummaryUserData(const PaymentTokenList& payment_tokens);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_SUMMARY_USER_DATA_H_
