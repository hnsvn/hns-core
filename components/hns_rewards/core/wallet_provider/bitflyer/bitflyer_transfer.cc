/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/wallet_provider/bitflyer/bitflyer_transfer.h"

#include <utility>

#include "hns/components/hns_rewards/core/bitflyer/bitflyer.h"
#include "hns/components/hns_rewards/core/bitflyer/bitflyer_util.h"
#include "hns/components/hns_rewards/core/endpoints/request_for.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {

using endpoints::PostCommitTransactionBitFlyer;
using endpoints::RequestFor;

namespace bitflyer {

void BitFlyerTransfer::CommitTransaction(
    ResultCallback callback,
    mojom::ExternalTransactionPtr transaction) const {
  if (!transaction) {
    return std::move(callback).Run(mojom::Result::FAILED);
  }

  DCHECK(!transaction->transaction_id.empty());

  const auto wallet =
      engine_->bitflyer()->GetWalletIf({mojom::WalletStatus::kConnected});
  if (!wallet) {
    return std::move(callback).Run(mojom::Result::FAILED);
  }

  auto on_commit_transaction =
      base::BindOnce(&BitFlyerTransfer::OnCommitTransaction,
                     base::Unretained(this), std::move(callback));

  RequestFor<PostCommitTransactionBitFlyer>(*engine_, std::move(wallet->token),
                                            std::move(wallet->address),
                                            std::move(transaction))
      .Send(std::move(on_commit_transaction));
}

void BitFlyerTransfer::OnCommitTransaction(
    ResultCallback callback,
    PostCommitTransactionBitFlyer::Result&& result) const {
  if (!engine_->bitflyer()->GetWalletIf({mojom::WalletStatus::kConnected})) {
    return std::move(callback).Run(mojom::Result::FAILED);
  }

  if (!result.has_value()) {
    if (result.error() ==
        PostCommitTransactionBitFlyer::Error::kAccessTokenExpired) {
      if (!engine_->bitflyer()->LogOutWallet()) {
        BLOG(0, "Failed to disconnect " << constant::kWalletBitflyer
                                        << " wallet!");
      }
    }

    return std::move(callback).Run(mojom::Result::FAILED);
  }

  std::move(callback).Run(mojom::Result::OK);
}

}  // namespace bitflyer

}  // namespace hns_rewards::internal
