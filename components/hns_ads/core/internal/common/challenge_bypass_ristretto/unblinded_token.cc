/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"

namespace hns_ads::cbr {

namespace {

absl::optional<challenge_bypass_ristretto::UnblindedToken> Create(
    const std::string& unblinded_token_base64) {
  if (unblinded_token_base64.empty()) {
    return absl::nullopt;
  }

  return ValueOrLogError(
      challenge_bypass_ristretto::UnblindedToken::decode_base64(
          unblinded_token_base64));
}

}  // namespace

UnblindedToken::UnblindedToken() = default;

UnblindedToken::UnblindedToken(const std::string& unblinded_token_base64)
    : unblinded_token_(Create(unblinded_token_base64)) {}

UnblindedToken::UnblindedToken(
    const challenge_bypass_ristretto::UnblindedToken& unblinded_token)
    : unblinded_token_(unblinded_token) {}

UnblindedToken::UnblindedToken(const UnblindedToken& other) = default;

UnblindedToken& UnblindedToken::operator=(const UnblindedToken& other) =
    default;

UnblindedToken::UnblindedToken(UnblindedToken&& other) noexcept = default;

UnblindedToken& UnblindedToken::operator=(UnblindedToken&& other) noexcept =
    default;

UnblindedToken::~UnblindedToken() = default;

bool UnblindedToken::operator==(const UnblindedToken& other) const {
  return EncodeBase64().value_or("") == other.EncodeBase64().value_or("");
}

bool UnblindedToken::operator!=(const UnblindedToken& other) const {
  return !(*this == other);
}

UnblindedToken UnblindedToken::DecodeBase64(
    const std::string& unblinded_token_base64) {
  return UnblindedToken(unblinded_token_base64);
}

absl::optional<std::string> UnblindedToken::EncodeBase64() const {
  if (!unblinded_token_ || !has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError(unblinded_token_->encode_base64());
}

absl::optional<VerificationKey> UnblindedToken::DeriveVerificationKey() const {
  if (!unblinded_token_ || !has_value()) {
    return absl::nullopt;
  }

  return VerificationKey(unblinded_token_->derive_verification_key());
}

absl::optional<TokenPreimage> UnblindedToken::GetTokenPreimage() const {
  if (!unblinded_token_ || !has_value()) {
    return absl::nullopt;
  }

  return TokenPreimage(unblinded_token_->preimage());
}

std::ostream& operator<<(std::ostream& os,
                         const UnblindedToken& unblinded_token) {
  os << unblinded_token.EncodeBase64().value_or("");
  return os;
}

}  // namespace hns_ads::cbr
