/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_factory.h"

#include <utility>

#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/redeem_non_reward_confirmation.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/redeem_reward_confirmation.h"

namespace hns_ads {

// static
void RedeemConfirmationFactory::BuildAndRedeemConfirmation(
    base::WeakPtr<RedeemConfirmationDelegate> delegate,
    const ConfirmationInfo& confirmation) {
  if (!confirmation.reward) {
    return RedeemNonRewardConfirmation::CreateAndRedeem(std::move(delegate),
                                                        confirmation);
  }

  return RedeemRewardConfirmation::CreateAndRedeem(std::move(delegate),
                                                   confirmation);
}

}  // namespace hns_ads
