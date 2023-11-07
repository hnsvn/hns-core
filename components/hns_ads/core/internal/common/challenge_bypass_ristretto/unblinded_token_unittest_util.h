/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_UNITTEST_UTIL_H_

#include <vector>

namespace hns_ads::cbr {

class UnblindedToken;

UnblindedToken GetUnblindedTokenForTesting();
std::vector<UnblindedToken> GetUnblindedTokensForTesting();

}  // namespace hns_ads::cbr

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_UNITTEST_UTIL_H_
