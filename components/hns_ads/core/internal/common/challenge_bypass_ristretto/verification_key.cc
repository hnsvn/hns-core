/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_signature.h"

namespace hns_ads::cbr {

VerificationKey::VerificationKey(
    const challenge_bypass_ristretto::VerificationKey& verification_key)
    : verification_key_(verification_key) {}

absl::optional<VerificationSignature> VerificationKey::Sign(
    const std::string& message) {
  return ValueOrLogError<challenge_bypass_ristretto::VerificationSignature,
                         VerificationSignature>(
      verification_key_.sign(message));
}

bool VerificationKey::Verify(
    const VerificationSignature& verification_signature,
    const std::string& message) {
  if (!verification_signature.has_value()) {
    return false;
  }

  return verification_key_.verify(verification_signature.get(), message)
      .value_or(false);
}

}  // namespace hns_ads::cbr
