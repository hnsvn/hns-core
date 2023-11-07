/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token_unittest_util.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token.h"

namespace hns_ads::cbr {

SignedToken GetSignedTokenForTesting() {
  return SignedToken(kSignedTokenBase64);
}

SignedToken GetInvalidSignedTokenForTesting() {
  return SignedToken(kInvalidBase64);
}

std::vector<SignedToken> GetSignedTokensForTesting() {
  return {GetSignedTokenForTesting()};
}

std::vector<SignedToken> GetInvalidSignedTokensForTesting() {
  return {GetInvalidSignedTokenForTesting()};
}

}  // namespace hns_ads::cbr
