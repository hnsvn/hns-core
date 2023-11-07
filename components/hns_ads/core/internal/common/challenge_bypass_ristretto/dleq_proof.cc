/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/dleq_proof.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signing_key.h"

namespace hns_ads::cbr {

namespace {

absl::optional<challenge_bypass_ristretto::DLEQProof> Create(
    const BlindedToken& blinded_token,
    const SignedToken& signed_token,
    const SigningKey& signing_key) {
  if (!blinded_token.has_value() || !signed_token.has_value() ||
      !signing_key.has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError(challenge_bypass_ristretto::DLEQProof::Create(
      blinded_token.get(), signed_token.get(), signing_key.get()));
}

absl::optional<challenge_bypass_ristretto::DLEQProof> Create(
    const std::string& dleq_proof_base64) {
  if (dleq_proof_base64.empty()) {
    return absl::nullopt;
  }

  return ValueOrLogError(
      challenge_bypass_ristretto::DLEQProof::decode_base64(dleq_proof_base64));
}

}  // namespace

DLEQProof::DLEQProof() = default;

DLEQProof::DLEQProof(const std::string& dleq_proof_base64)
    : dleq_proof_(Create(dleq_proof_base64)) {}

DLEQProof::DLEQProof(const BlindedToken& blinded_token,
                     const SignedToken& signed_token,
                     const SigningKey& signing_key)
    : dleq_proof_(Create(blinded_token, signed_token, signing_key)) {}

DLEQProof::~DLEQProof() = default;

bool DLEQProof::operator==(const DLEQProof& other) const {
  return EncodeBase64().value_or("") == other.EncodeBase64().value_or("");
}

bool DLEQProof::operator!=(const DLEQProof& other) const {
  return !(*this == other);
}

DLEQProof DLEQProof::DecodeBase64(const std::string& dleq_proof_base64) {
  return DLEQProof(dleq_proof_base64);
}

absl::optional<std::string> DLEQProof::EncodeBase64() const {
  if (!dleq_proof_ || !has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError(dleq_proof_->encode_base64());
}

bool DLEQProof::Verify(const BlindedToken& blinded_token,
                       const SignedToken& signed_token,
                       const PublicKey& public_key) {
  if (!dleq_proof_ || !has_value() || !blinded_token.has_value() ||
      !signed_token.has_value() || !public_key.has_value()) {
    return false;
  }

  return ValueOrLogError(dleq_proof_->verify(blinded_token.get(),
                                             signed_token.get(),
                                             public_key.get()))
      .value_or(false);
}

std::ostream& operator<<(std::ostream& os, const DLEQProof& dleq_proof) {
  os << dleq_proof.EncodeBase64().value_or("");
  return os;
}

}  // namespace hns_ads::cbr
