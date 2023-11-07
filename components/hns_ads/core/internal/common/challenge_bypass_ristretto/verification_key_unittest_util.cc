/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key_unittest_util.h"

#include "base/check.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::cbr {

VerificationKey GetVerificationKeyForTesting() {
  const UnblindedToken unblinded_token = GetUnblindedTokenForTesting();
  const absl::optional<VerificationKey> verification_key =
      unblinded_token.DeriveVerificationKey();
  CHECK(verification_key);
  return *verification_key;
}

}  // namespace hns_ads::cbr
