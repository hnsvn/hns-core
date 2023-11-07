/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_BITFLYER_TRANSFER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_BITFLYER_TRANSFER_H_

#include "hns/components/hns_rewards/core/endpoints/bitflyer/post_commit_transaction_bitflyer.h"
#include "hns/components/hns_rewards/core/wallet_provider/transfer.h"

namespace hns_rewards::internal::bitflyer {

class BitFlyerTransfer final : public wallet_provider::Transfer {
 public:
  using Transfer::Transfer;

 private:
  void CommitTransaction(ResultCallback,
                         mojom::ExternalTransactionPtr) const override;

  void OnCommitTransaction(
      ResultCallback,
      endpoints::PostCommitTransactionBitFlyer::Result&&) const;
};

}  // namespace hns_rewards::internal::bitflyer

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_BITFLYER_BITFLYER_TRANSFER_H_
