/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token_unittest_util.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"

namespace hns_ads::cbr {

UnblindedToken GetUnblindedTokenForTesting() {
  return UnblindedToken(kUnblindedTokenBase64);
}

std::vector<UnblindedToken> GetUnblindedTokensForTesting() {
  std::vector<UnblindedToken> unblinded_tokens;
  const UnblindedToken unblinded_token = GetUnblindedTokenForTesting();
  unblinded_tokens.push_back(unblinded_token);
  return unblinded_tokens;
}

}  // namespace hns_ads::cbr
