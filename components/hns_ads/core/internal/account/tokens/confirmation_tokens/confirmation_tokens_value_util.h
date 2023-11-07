/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_VALUE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_VALUE_UTIL_H_

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_token_info.h"

#include "base/values.h"

namespace hns_ads {

base::Value::List ConfirmationTokensToValue(
    const ConfirmationTokenList& confirmation_tokens);

ConfirmationTokenList ConfirmationTokensFromValue(
    const base::Value::List& list);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_VALUE_UTIL_H_
