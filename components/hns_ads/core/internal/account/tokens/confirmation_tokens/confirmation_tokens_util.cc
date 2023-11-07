/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_util.h"

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens.h"
#include "hns/components/hns_ads/core/internal/deprecated/confirmations/confirmation_state_manager.h"

namespace hns_ads {

namespace {

bool HasConfirmationTokens() {
  return ConfirmationTokenCount() > 0;
}

}  // namespace

absl::optional<ConfirmationTokenInfo> MaybeGetConfirmationToken() {
  if (!HasConfirmationTokens()) {
    return absl::nullopt;
  }

  return ConfirmationStateManager::GetInstance()
      .GetConfirmationTokens()
      .GetToken();
}

void AddConfirmationTokens(const ConfirmationTokenList& confirmation_tokens) {
  ConfirmationStateManager::GetInstance().GetConfirmationTokens().AddTokens(
      confirmation_tokens);

  ConfirmationStateManager::GetInstance().Save();
}

bool RemoveConfirmationToken(const ConfirmationTokenInfo& confirmation_token) {
  if (!ConfirmationStateManager::GetInstance()
           .GetConfirmationTokens()
           .RemoveToken(confirmation_token)) {
    return false;
  }

  ConfirmationStateManager::GetInstance().Save();

  return true;
}

void RemoveConfirmationTokens(
    const ConfirmationTokenList& confirmation_tokens) {
  ConfirmationStateManager::GetInstance().GetConfirmationTokens().RemoveTokens(
      confirmation_tokens);

  ConfirmationStateManager::GetInstance().Save();
}

void RemoveAllConfirmationTokens() {
  ConfirmationStateManager::GetInstance()
      .GetConfirmationTokens()
      .RemoveAllTokens();

  ConfirmationStateManager::GetInstance().Save();
}

bool ConfirmationTokenExists(const ConfirmationTokenInfo& confirmation_token) {
  return ConfirmationStateManager::GetInstance()
      .GetConfirmationTokens()
      .TokenExists(confirmation_token);
}

bool ConfirmationTokensIsEmpty() {
  return ConfirmationStateManager::GetInstance()
      .GetConfirmationTokens()
      .IsEmpty();
}

int ConfirmationTokenCount() {
  return ConfirmationStateManager::GetInstance()
      .GetConfirmationTokens()
      .Count();
}

bool IsValid(const ConfirmationTokenInfo& confirmation_token) {
  return confirmation_token.unblinded_token.has_value() &&
         confirmation_token.public_key.has_value() &&
         !confirmation_token.signature.empty();
}

}  // namespace hns_ads
