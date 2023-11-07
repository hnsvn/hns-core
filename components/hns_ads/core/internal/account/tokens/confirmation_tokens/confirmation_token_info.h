/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKEN_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKEN_INFO_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"

namespace hns_ads {

struct ConfirmationTokenInfo final {
  bool operator==(const ConfirmationTokenInfo&) const;
  bool operator!=(const ConfirmationTokenInfo&) const;

  cbr::UnblindedToken unblinded_token;
  cbr::PublicKey public_key;
  std::string signature;
};

using ConfirmationTokenList = std::vector<ConfirmationTokenInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKEN_INFO_H_
