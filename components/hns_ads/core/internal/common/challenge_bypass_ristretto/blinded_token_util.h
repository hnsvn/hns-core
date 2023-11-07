/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_BLINDED_TOKEN_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_BLINDED_TOKEN_UTIL_H_

#include <vector>

#include "hns/third_party/challenge_bypass_ristretto_ffi/src/wrapper.h"

namespace hns_ads::cbr {

class BlindedToken;
class Token;

std::vector<BlindedToken> BlindTokens(const std::vector<Token>& tokens);

std::vector<challenge_bypass_ristretto::BlindedToken> ToRawBlindedTokens(
    const std::vector<BlindedToken>& tokens);

}  // namespace hns_ads::cbr

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_BLINDED_TOKEN_UTIL_H_
