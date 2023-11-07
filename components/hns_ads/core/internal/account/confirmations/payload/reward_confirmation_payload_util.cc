/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/payload/reward_confirmation_payload_util.h"

#include <string>
#include <utility>

#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

namespace {

constexpr char kBlindedTokensKey[] = "blindedPaymentTokens";
constexpr char kPublicKeyKey[] = "publicKey";

}  // namespace

base::Value::Dict BuildRewardConfirmationPayload(const RewardInfo& reward) {
  base::Value::List list;
  const absl::optional<std::string> blinded_token_base64 =
      reward.blinded_token.EncodeBase64();
  CHECK(blinded_token_base64);
  list.Append(*blinded_token_base64);

  const absl::optional<std::string> public_key_base64 =
      reward.public_key.EncodeBase64();
  CHECK(public_key_base64);

  return base::Value::Dict()
      .Set(kBlindedTokensKey, std::move(list))
      .Set(kPublicKeyKey, *public_key_base64);
}

}  // namespace hns_ads
