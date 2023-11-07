/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_credential_json_writer.h"

#include "base/check.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_info.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_signature.h"

namespace hns_ads::json::writer {

namespace {

constexpr char kVerificationSignatureKey[] = "signature";
constexpr char kTokenPreimageKey[] = "t";

}  // namespace

absl::optional<std::string> WriteRewardCredential(
    const absl::optional<RewardInfo>& reward,
    const std::string& payload) {
  CHECK(!payload.empty());

  if (!reward) {
    return absl::nullopt;
  }

  absl::optional<cbr::VerificationKey> verification_key =
      reward->unblinded_token.DeriveVerificationKey();
  if (!verification_key) {
    return absl::nullopt;
  }

  const absl::optional<cbr::VerificationSignature> verification_signature =
      verification_key->Sign(payload);
  if (!verification_signature) {
    return absl::nullopt;
  }

  const absl::optional<std::string> verification_signature_base64 =
      verification_signature->EncodeBase64();
  if (!verification_signature_base64) {
    return absl::nullopt;
  }

  const absl::optional<cbr::TokenPreimage> token_preimage =
      reward->unblinded_token.GetTokenPreimage();
  if (!token_preimage) {
    return absl::nullopt;
  }

  const absl::optional<std::string> token_preimage_base64 =
      token_preimage->EncodeBase64();
  if (!token_preimage_base64) {
    return absl::nullopt;
  }

  const auto dict =
      base::Value::Dict()
          .Set(kVerificationSignatureKey, *verification_signature_base64)
          .Set(kTokenPreimageKey, *token_preimage_base64);

  std::string json;
  CHECK(base::JSONWriter::Write(dict, &json));
  return json;
}

}  // namespace hns_ads::json::writer
