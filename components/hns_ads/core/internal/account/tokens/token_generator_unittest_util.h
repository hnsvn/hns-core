/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_TOKEN_GENERATOR_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_TOKEN_GENERATOR_UNITTEST_UTIL_H_

#include <vector>

#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_mock.h"

namespace hns_ads {

namespace cbr {
class Token;
}  // namespace cbr

void MockTokenGenerator(const TokenGeneratorMock& mock, size_t count);

std::vector<cbr::Token> BuildTokensForTesting(size_t count);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_TOKEN_GENERATOR_UNITTEST_UTIL_H_
