/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_UPHOLD_UPHOLD_TRANSFER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_UPHOLD_UPHOLD_TRANSFER_H_

#include <string>

#include "hns/components/hns_rewards/core/endpoints/uphold/get_transaction_status_uphold.h"
#include "hns/components/hns_rewards/core/endpoints/uphold/post_commit_transaction_uphold.h"
#include "hns/components/hns_rewards/core/endpoints/uphold/post_create_transaction_uphold.h"
#include "hns/components/hns_rewards/core/wallet_provider/transfer.h"

namespace hns_rewards::internal::uphold {

class UpholdTransfer final : public wallet_provider::Transfer {
 public:
  using Transfer::Transfer;

 private:
  void CreateTransaction(MaybeCreateTransactionCallback,
                         mojom::ExternalTransactionPtr) const override;

  void OnCreateTransaction(
      MaybeCreateTransactionCallback,
      mojom::ExternalTransactionPtr,
      endpoints::PostCreateTransactionUphold::Result&&) const;

  void CommitTransaction(ResultCallback,
                         mojom::ExternalTransactionPtr) const override;

  void OnCommitTransaction(
      ResultCallback,
      std::string&& transaction_id,
      endpoints::PostCommitTransactionUphold::Result&&) const;

  void OnGetTransactionStatus(
      ResultCallback,
      endpoints::GetTransactionStatusUphold::Result&&) const;
};

}  // namespace hns_rewards::internal::uphold

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_UPHOLD_UPHOLD_TRANSFER_H_
