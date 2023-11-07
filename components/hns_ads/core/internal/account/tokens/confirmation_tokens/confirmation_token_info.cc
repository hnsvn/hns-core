/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_token_info.h"

#include <tuple>

namespace hns_ads {

bool ConfirmationTokenInfo::operator==(
    const ConfirmationTokenInfo& other) const {
  const auto tie = [](const ConfirmationTokenInfo& confirmation_token) {
    return std::tie(confirmation_token.unblinded_token,
                    confirmation_token.public_key,
                    confirmation_token.signature);
  };

  return tie(*this) == tie(other);
}

bool ConfirmationTokenInfo::operator!=(
    const ConfirmationTokenInfo& other) const {
  return !(*this == other);
}

}  // namespace hns_ads
