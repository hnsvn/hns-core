/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REFILL_CONFIRMATION_TOKENS_URL_REQUESTS_GET_SIGNED_TOKENS_GET_SIGNED_TOKENS_URL_REQUEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REFILL_CONFIRMATION_TOKENS_URL_REQUESTS_GET_SIGNED_TOKENS_GET_SIGNED_TOKENS_URL_REQUEST_UTIL_H_

#include <string>
#include <tuple>
#include <vector>

#include "base/types/expected.h"
#include "base/values.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

namespace cbr {
class BlindedToken;
class PublicKey;
class Token;
class UnblindedToken;
}  // namespace cbr

struct WalletInfo;

absl::optional<std::string> ParseCaptchaId(const base::Value::Dict& dict);

base::expected<std::tuple<std::vector<cbr::UnblindedToken>, cbr::PublicKey>,
               std::string>
ParseAndUnblindSignedTokens(
    const base::Value::Dict& dict,
    const std::vector<cbr::Token>& tokens,
    const std::vector<cbr::BlindedToken>& blinded_tokens);

void BuildAndAddConfirmationTokens(
    const std::vector<cbr::UnblindedToken>& unblinded_tokens,
    const cbr::PublicKey& public_key,
    const WalletInfo& wallet);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REFILL_CONFIRMATION_TOKENS_URL_REQUESTS_GET_SIGNED_TOKENS_GET_SIGNED_TOKENS_URL_REQUEST_UTIL_H_
