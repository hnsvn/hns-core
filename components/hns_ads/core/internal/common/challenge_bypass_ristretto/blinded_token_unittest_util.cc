/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token_unittest_util.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"

namespace hns_ads::cbr {

BlindedToken GetBlindedTokenForTesting() {
  return BlindedToken(kBlindedTokenBase64);
}

BlindedToken GetInvalidBlindedTokenForTesting() {
  return BlindedToken(kInvalidBase64);
}

std::vector<BlindedToken> GetBlindedTokensForTesting() {
  return {GetBlindedTokenForTesting()};
}

std::vector<BlindedToken> GetInvalidBlindedTokensForTesting() {
  return {GetInvalidBlindedTokenForTesting()};
}

}  // namespace hns_ads::cbr
