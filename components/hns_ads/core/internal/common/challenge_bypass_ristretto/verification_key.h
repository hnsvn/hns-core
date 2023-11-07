/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_KEY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_KEY_H_

#include <string>

#include "hns/third_party/challenge_bypass_ristretto_ffi/src/wrapper.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::cbr {

class VerificationSignature;

// A shared |VerificationKey| for proving and verifying the validity of an
// |UnblindedToken|. (K = H_2(t, W)).

class VerificationKey {
 public:
  explicit VerificationKey(
      const challenge_bypass_ristretto::VerificationKey& verification_key);

  challenge_bypass_ristretto::VerificationKey& get() {
    return verification_key_;
  }

  const challenge_bypass_ristretto::VerificationKey& get() const {
    return verification_key_;
  }

  absl::optional<VerificationSignature> Sign(const std::string& message);

  [[nodiscard]] bool Verify(const VerificationSignature& verification_signature,
                            const std::string& message);

 private:
  challenge_bypass_ristretto::VerificationKey verification_key_;
};

}  // namespace hns_ads::cbr

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_KEY_H_
