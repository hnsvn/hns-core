/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_util.h"

namespace hns_ads::cbr {

namespace {

absl::optional<challenge_bypass_ristretto::TokenPreimage> Create(
    const std::string& token_preimage_base64) {
  if (token_preimage_base64.empty()) {
    return absl::nullopt;
  }

  return ValueOrLogError(
      challenge_bypass_ristretto::TokenPreimage::decode_base64(
          token_preimage_base64));
}

}  // namespace

TokenPreimage::TokenPreimage() = default;

TokenPreimage::TokenPreimage(const std::string& token_preimage_base64)
    : token_preimage_(Create(token_preimage_base64)) {}

TokenPreimage::TokenPreimage(
    const challenge_bypass_ristretto::TokenPreimage& token_preimage)
    : token_preimage_(token_preimage) {}

TokenPreimage::TokenPreimage(const TokenPreimage& other) = default;

TokenPreimage& TokenPreimage::operator=(const TokenPreimage& other) = default;

TokenPreimage::TokenPreimage(TokenPreimage&& other) noexcept = default;

TokenPreimage& TokenPreimage::operator=(TokenPreimage&& other) noexcept =
    default;

TokenPreimage::~TokenPreimage() = default;

bool TokenPreimage::operator==(const TokenPreimage& other) const {
  return EncodeBase64().value_or("") == other.EncodeBase64().value_or("");
}

bool TokenPreimage::operator!=(const TokenPreimage& other) const {
  return !(*this == other);
}

TokenPreimage TokenPreimage::DecodeBase64(
    const std::string& token_preimage_base64) {
  return TokenPreimage(token_preimage_base64);
}

absl::optional<std::string> TokenPreimage::EncodeBase64() const {
  if (!token_preimage_ || !has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError(token_preimage_->encode_base64());
}

std::ostream& operator<<(std::ostream& os,
                         const TokenPreimage& token_preimage) {
  os << token_preimage.EncodeBase64().value_or("");
  return os;
}

}  // namespace hns_ads::cbr
