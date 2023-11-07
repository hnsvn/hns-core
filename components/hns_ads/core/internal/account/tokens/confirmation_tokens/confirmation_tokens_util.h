/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_UTIL_H_

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_token_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

absl::optional<ConfirmationTokenInfo> MaybeGetConfirmationToken();

void AddConfirmationTokens(const ConfirmationTokenList& confirmation_tokens);

bool RemoveConfirmationToken(const ConfirmationTokenInfo& confirmation_token);
void RemoveConfirmationTokens(const ConfirmationTokenList& confirmation_tokens);
void RemoveAllConfirmationTokens();

bool ConfirmationTokenExists(const ConfirmationTokenInfo& confirmation_token);

bool ConfirmationTokensIsEmpty();

int ConfirmationTokenCount();

[[nodiscard]] bool IsValid(const ConfirmationTokenInfo& confirmation_token);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_UTIL_H_
