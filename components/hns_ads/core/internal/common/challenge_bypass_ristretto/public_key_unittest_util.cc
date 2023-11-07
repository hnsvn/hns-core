/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key_unittest_util.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"

namespace hns_ads::cbr {

PublicKey GetPublicKeyForTesting() {
  return PublicKey(kPublicKeyBase64);
}

PublicKey GetMismatchingPublicKeyForTesting() {
  return {};
}

PublicKey GetInvalidPublicKeyForTesting() {
  return PublicKey(kInvalidBase64);
}

}  // namespace hns_ads::cbr
