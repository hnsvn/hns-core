/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/non_reward/non_reward_confirmation_util.h"

#include "base/check.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/account/user_data/user_data_info.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

absl::optional<ConfirmationInfo> BuildNonRewardConfirmation(
    const TransactionInfo& transaction,
    const UserDataInfo& user_data) {
  CHECK(transaction.IsValid());
  CHECK(!UserHasJoinedHnsRewards());

  ConfirmationInfo confirmation;
  confirmation.transaction_id = transaction.id;
  confirmation.creative_instance_id = transaction.creative_instance_id;
  confirmation.type = transaction.confirmation_type;
  confirmation.ad_type = transaction.ad_type;
  confirmation.created_at = transaction.created_at;
  confirmation.user_data = user_data;

  return confirmation;
}

}  // namespace hns_ads
