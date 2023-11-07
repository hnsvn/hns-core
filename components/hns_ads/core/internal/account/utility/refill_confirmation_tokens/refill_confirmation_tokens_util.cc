/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/refill_confirmation_tokens_util.h"

#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/tokens_feature.h"

namespace hns_ads {

bool ShouldRefillConfirmationTokens() {
  return ConfirmationTokenCount() < kMinConfirmationTokens.Get();
}

int CalculateAmountOfConfirmationTokensToRefill() {
  return kMaxConfirmationTokens.Get() - ConfirmationTokenCount();
}

}  // namespace hns_ads
