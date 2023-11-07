/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_UTIL_H_

#include <memory>
#include <string>
#include <vector>

#include "base/types/expected.h"
#include "base/values.h"
#include "hns/components/hns_rewards/core/credentials/credentials_redeem.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

#include "hns/third_party/challenge_bypass_ristretto_ffi/src/wrapper.h"

using challenge_bypass_ristretto::BlindedToken;
using challenge_bypass_ristretto::Token;

namespace hns_rewards::internal {
namespace credential {

std::vector<Token> GenerateCreds(const int count);

std::string GetCredsJSON(const std::vector<Token>& creds);

std::vector<BlindedToken> GenerateBlindCreds(const std::vector<Token>& tokens);

std::string GetBlindedCredsJSON(const std::vector<BlindedToken>& blinded);

absl::optional<base::Value::List> ParseStringToBaseList(
    const std::string& string_list);

base::expected<std::vector<std::string>, std::string> UnBlindCreds(
    const mojom::CredsBatch& creds);

std::vector<std::string> UnBlindCredsMock(const mojom::CredsBatch& creds);

std::string ConvertRewardTypeToString(const mojom::RewardsType type);

base::Value::List GenerateCredentials(
    const std::vector<mojom::UnblindedToken>& token_list,
    const std::string& body);

absl::optional<base::Value::Dict> GenerateSuggestion(
    const std::string& token_value,
    const std::string& public_key,
    const std::string& suggestion_encoded);

base::Value::Dict GenerateSuggestionMock(const std::string& token_value,
                                         const std::string& public_key,
                                         const std::string& suggestion_encoded);

}  // namespace credential
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_UTIL_H_
